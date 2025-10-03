# ChipEight Emulator

A project dedicated towards learning and implementing a emulator that runs Chip 8 games and apps.  

Chip 8 is a interpreted programming language that was used to write various software/games and ran on several devices since the 1970's. 

The main goal with this emulator is to learn/implement: 
- Cpu and it's instructions 
- Graphics
- Keypad 
- Sound

Additional Features I may add: 

- A full GUI
- Customizations to Graphics and Sounds

### Installation
<a href = "https://jeremys22.github.io/ChipEightDocs/docs/installation"> So far only linux is listed </a>, but more will be added later. 

### Progress 
Note that these games are still a WIP 
![Recording 2025-10-01 221150_PONG_PUSHED](https://github.com/user-attachments/assets/6e6e690f-e925-4cdb-9da4-d956c4fde342)
<p align = "center"><strong><em>Pong</em></strong></p> 

![Recording 2025-09-23 122546_GIF_PUSHED](https://github.com/user-attachments/assets/065c4527-3ff7-456d-b914-33995d87f27f)
<p align = "center"><strong><em>Space Invaders</em></strong></p> 

![Recording 2025-09-27 221332_PUSHED_](https://github.com/user-attachments/assets/c0796fad-5202-46b4-8f72-3059681361d3)
<p align = "center"><strong><em>Missle</em></strong></p> 

### Debugger 
Can step through instructions or run debugger in real time without stepping 
![Recording 2025-10-02 234650_DEBUGGER_DEMO_PUSHED_VERSION_2](https://github.com/user-attachments/assets/bb0a5d21-a06a-4750-8cb3-c892dfedc77c)
<p align = "center"><strong><em>Debugger Implemented Previewing CPU Opcodes, Stack, Program Counter, Registers </em></strong></p> 

### Test Roms 
Here's a list of fully working ROMs that assess various components of the Chip 8 emulator (ie. graphics, opcodes, flags, IO, etc.) and ensures each functions correctly 

![Screenshot 2025-07-02 153850](https://github.com/user-attachments/assets/0efe8489-3846-4030-8038-de5adbc142ad)
<p align = "center"><strong><em>IBM Boot Logo rom fully rendered  
  <br>and utilizes a few basic instructions (including rendering, jumping and a few other opcodes) </em></strong></p> 

<img width="1065" height="548" alt="Screenshot 2025-07-20 163218" src="https://github.com/user-attachments/assets/15543fcc-9069-4d85-8850-3ddeb20cd6db" />
<p align = "center"><strong><em>Corax's Opcode Test ROM that Test a good Portion of the Chip 8's Instruction Set</em></strong></p>

<strong>ROM source:</strong><br>
https://github.com/corax89/chip8-test-rom 

![Screenshot 2025-07-05 ](https://github.com/user-attachments/assets/80045ea2-452f-48b2-ba56-1c59406104b8)
<p align = "center"><strong><em>Timendus' Chip 8 splash screen rom</em></strong></p>

<img width="1103" height="553" alt="Screenshot 2025-07-22 163218" src="https://github.com/user-attachments/assets/cd95e96c-50c3-45b5-99dd-6565d06c38bd" />
<p align = "center"><strong><em>Timendus' Opcode Test ROM </em></strong></p> 
<p align = "center">This is Timendus' version of Corax's original Opcode test ROM and tests the functionality of even more opcodes.  <br>(The "r" next to the opcodes are checkmarks)</p>

<img width="1066" height="557" alt="image" src="https://github.com/user-attachments/assets/5fa3fa1e-80d4-4627-8aae-6924939b2fbe" />
<p align = "center"><strong><em>Timendus' Flag ROM that tests Chip 8's flag register</em></strong></p> 

 
<strong>ROM source:</strong><br>
https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file 

Thanks to Timendus and Corax for these very helpful resources 😊<br>  





