/*
	olc::NES - Part #5 - PPU Rendering - Foregrounds
	"Thanks Dad for believing computers were gonna be a big deal..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


	Relevant Video: https://youtu.be/cksywUTZxlY

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2019
*/

#include <iostream>
#include <sstream>
#include <deque>
#include <vector>

#include "Bus.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// OpenAL for macOS audio support
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif


class Demo_olcNES : public olc::PixelGameEngine
{
public:
	Demo_olcNES() { sAppName = "olcNES Sound Demonstration"; }
	Demo_olcNES(const std::string& romPath) : sRomPath(romPath) { sAppName = "olcNES Sound Demonstration"; }

private:
	std::string sRomPath = "./nestest.nes";

	// The NES
	Bus nes;
	std::shared_ptr<Cartridge> cart;
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;

	uint8_t nSelectedPalette = 0x00;

	std::list<uint16_t> audio[4];
	float fAccumulatedTime = 0.0f;

	// OpenAL audio members
	ALCdevice* al_device = nullptr;
	ALCcontext* al_context = nullptr;
	ALuint al_source = 0;
	int al_status = 0;
	std::vector<uint8_t> audio_buffer;

	// Audio sampling control
	int audio_sample_counter = 0;
	const int audio_sample_interval = 120; // Sample every N clocks to get ~735 samples per frame (29780 clocks / 735 ≈ 40, but adjust for actual)

private: 
	// Support Utilities
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawString(x , y , "STATUS:", olc::WHITE);
		DrawString(x  + 64, y, "N", nes.cpu.status & olc6502::N ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "V", nes.cpu.status & olc6502::V ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "-", nes.cpu.status & olc6502::U ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "B", nes.cpu.status & olc6502::B ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "D", nes.cpu.status & olc6502::D ? olc::GREEN : olc::RED);
		DrawString(x  + 144, y , "I", nes.cpu.status & olc6502::I ? olc::GREEN : olc::RED);
		DrawString(x  + 160, y , "Z", nes.cpu.status & olc6502::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 178, y , "C", nes.cpu.status & olc6502::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x , y + 20, "A: $" +  hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x , y + 30, "X: $" +  hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x , y + 40, "Y: $" +  hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x , y + 50, "Stack P: $" + hex(nes.cpu.stkp, 4));
	}

	void DrawCode(int x, int y, int nLines)
	{
		auto it_a = mapAsm.find(nes.cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}

	void DrawAudio(int channel, int x, int y)
	{
		FillRect(x, y, 120, 120, olc::BLACK);
		int i = 0;
		for (auto s : audio[channel])
		{
			Draw(x + i, y + (s >> (channel == 2 ? 5 : 4)), olc::YELLOW);
			i++;
		}
	}

	bool OnUserCreate() override
	{
		// Load the cartridge
		cart = std::make_shared<Cartridge>(sRomPath);

		if (!cart->ImageValid())
			return false;

		// Insert into NES
		nes.insertCartridge(cart);

		// Extract dissassembly
		//mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 120; j++)
				audio[i].push_back(0);
		}

		// Reset NES
		nes.reset();

		// Initialize OpenAL for real NES audio
		std::cout << "🎵 Initializing OpenAL audio..." << std::endl;
		al_device = alcOpenDevice(NULL);

		if (!al_device) {
			std::cout << "⚠️  Failed to open OpenAL device, running in silent mode" << std::endl;
		} else {
			al_context = alcCreateContext(al_device, NULL);
			if (!al_context) {
				std::cout << "⚠️  Failed to create OpenAL context" << std::endl;
				alcCloseDevice(al_device);
				al_device = nullptr;
			} else {
				alcMakeContextCurrent(al_context);
				alGenSources(1, &al_source);
				al_status = 0;
				std::cout << "✓ OpenAL audio initialized successfully!" << std::endl;
			}
		}

		// Set audio sample frequency
		nes.SetSampleFrequency(44100);

		return true;
	}

	// Clean up OpenAL resources
	bool OnUserDestroy() override
	{
		if (al_source) {
			alDeleteSources(1, &al_source);
		}
		if (al_context) {
			alcMakeContextCurrent(NULL);
			alcDestroyContext(al_context);
		}
		if (al_device) {
			alcCloseDevice(al_device);
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		EmulatorUpdateWithAudio(fElapsedTime);
		return true;
	}

	// Submit audio samples to OpenAL
	void SubmitAudioToOpenAL()
	{
		if (!al_device || !al_source) return;
		if (audio_buffer.empty()) return;

		// Create and fill OpenAL buffer with accumulated samples
		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, AL_FORMAT_MONO8,
					audio_buffer.data(), audio_buffer.size(), 44100);
		alSourceQueueBuffers(al_source, 1, &buffer);

		// Clean up processed buffers to prevent memory leak
		ALint processed;
		alGetSourcei(al_source, AL_BUFFERS_PROCESSED, &processed);
		if (processed > 0) {
			ALuint processed_buffers[16];
			int to_unqueue = (processed > 16) ? 16 : processed;
			alSourceUnqueueBuffers(al_source, to_unqueue, processed_buffers);
			alDeleteBuffers(to_unqueue, processed_buffers);
		}

		// Manage playback state based on buffer queue depth
		ALint queued;
		alGetSourcei(al_source, AL_BUFFERS_QUEUED, &queued);

		// Start playing when we have enough buffered audio
		if (queued >= 3 && al_status == 0) {
			al_status = 1;
			alSourcePlay(al_source);
			std::cout << "🎵 Audio playback started (queue: " << queued << ", samples: " << audio_buffer.size() << ")" << std::endl;
		}
		// Pause if queue is too shallow to prevent crackling
		else if (queued <= 1 && al_status == 1) {
			al_status = 0;
			alSourcePause(al_source);
			std::cout << "⏸️  Audio paused (queue: " << queued << ")" << std::endl;
		}

		audio_buffer.clear();
	}

	// This performs an emulation update but synced to audio, so it cant
	// perform stepping through code or frames. Essentially, it runs
	// the emulation in real time now, so only accepts "controller" input
	// and updates the display
	bool EmulatorUpdateWithAudio(float fElapsedTime)
	{
		// Sample audio channel output roughly once per frame
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= 1.0f / 60.0f)
		{
			fAccumulatedTime -= (1.0f / 60.0f);
			audio[0].pop_front();
			audio[0].push_back(nes.apu.pulse1_visual);
			audio[1].pop_front();
			audio[1].push_back(nes.apu.pulse2_visual);
			audio[2].pop_front();
			audio[2].push_back(nes.apu.noise_visual);
		}

		// Run emulation for one frame
		if (fResidualTime > 0.0f)
			fResidualTime -= fElapsedTime;
		else
		{
			fResidualTime += (1.0f / 60.0f) - fElapsedTime;

			// Accumulate audio samples during frame execution
			do {
				nes.clock();

				// Sample audio at correct rate (~735 samples per frame)
				audio_sample_counter++;
				if (audio_sample_counter >= audio_sample_interval) {
					audio_sample_counter = 0;
					double audio_sample = nes.dAudioSample;
					if (audio_sample < -1.0) audio_sample = -1.0;
					if (audio_sample > 1.0) audio_sample = 1.0;
					uint8_t sample = static_cast<uint8_t>((audio_sample + 1.0) * 127.5);
					audio_buffer.push_back(sample);
				}
			} while (!nes.ppu.frame_complete);
			nes.ppu.frame_complete = false;

			// Submit accumulated audio after frame completes
			SubmitAudioToOpenAL();
		}

		Clear(olc::DARK_BLUE);

		// Handle input for controller in port #1
		nes.controller[0] = 0x00;
		nes.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;     // A Button
		nes.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;     // B Button
		nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;     // Select
		nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;     // Start
		nes.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
		nes.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

		if (GetKey(olc::Key::R).bPressed) nes.reset();
		if (GetKey(olc::Key::P).bPressed) (++nSelectedPalette) &= 0x07;

		DrawCpu(516, 2);
		//DrawCode(516, 72, 26);

		// Draw OAM Contents (first 26 out of 64) ======================================
		/*for (int i = 0; i < 26; i++)
		{
			std::string s = hex(i, 2) + ": (" + std::to_string(nes.ppu.pOAM[i * 4 + 3])
				+ ", " + std::to_string(nes.ppu.pOAM[i * 4 + 0]) + ") "
				+ "ID: " + hex(nes.ppu.pOAM[i * 4 + 1], 2) +
				+" AT: " + hex(nes.ppu.pOAM[i * 4 + 2], 2);
			DrawString(516, 72 + i * 10, s);
		}*/

		// Draw AUDIO Channels
		DrawAudio(0, 520, 72);
		DrawAudio(1, 644, 72);
		DrawAudio(2, 520, 196);
		DrawAudio(3, 644, 196);

		// Draw Palettes & Pattern Tables ==============================================
		const int nSwatchSize = 6;
		for (int p = 0; p < 8; p++) // For each palette
			for(int s = 0; s < 4; s++) // For each index
				FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, 
					nSwatchSize, nSwatchSize, nes.ppu.GetColourFromPaletteRam(p, s));
		
		// Draw selection reticule around selected palette
		DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

		// Generate Pattern Tables
		DrawSprite(516, 348, &nes.ppu.GetPatternTable(0, nSelectedPalette));
		DrawSprite(648, 348, &nes.ppu.GetPatternTable(1, nSelectedPalette));

		// Draw rendered output ========================================================
		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
		return true;
	}
};

int main(int argc, char* argv[])
{
	// Check for ROM path argument
	std::string romPath = "./nestest.nes"; // Default ROM path
	if (argc > 1)
	{
		romPath = argv[1];
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " <path/to/rom.nes>" << std::endl;
		std::cout << "Using default ROM: " << romPath << std::endl;
	}

	Demo_olcNES demo(romPath);
	demo.Construct(780, 480, 2, 2);
	demo.Start();
	return 0;
}