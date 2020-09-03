# Notes by mrwonko

First Person Synapse is planned to be created during the 2020 edition of the 7DFPS game jam. The initial idea dates back to the first 7DFPS in 2012: an FPS version of Frozen Synapse, wherein players record their button inputs for the next couple of seconds simultaneously, then submit them and see how it plays out.

I'm going to replace the entire game logic, since I need unwind functionality, i.e. probably event sourcing. Since I only need a fraction of the logic Jedi Academy usually has, that's probably faster than adjusting the existing code.

## Minimum Feature Set

* players with movement (run, jump)
* weapons
* multiplayer
  * peer-to-peer
    * digital signatures for anti-cheat
  * lobbies
  * matchmaking

## Extended Feature Set

* triggers
  * weapon & ammo pickups
  * jump pads
  * doors
  * killzones (lava?)
* flags
* replays

## Logo Idea

* the Frozen Synapse logo, but from a first-person perspective instead of a top-down view

## Code Exploration

* entry point: sys/sys_main.cpp
* input goes to Sys_QueEvent and is fetched via Sys_GetEvent <- Com_GetRealEvent <- Com_GetEvent <- Com_EventLoop, where dispatch happens.
* Com_Init for cvars, FS, config, memory, SV_Init, CL_Init etc.
* SE = StringEd, for localisation. I don't think the corresponding Editor has been released, but the code still has support for its ".ste" exports.
* Zone memory allocations get tagged, with the option to free all allocations with a certain tag.
* Hunk memory uses Zone memory, with one tag for allocations before the "mark" (level and game VM are loaded) and one for afterwards (gets cleared on vid_restart/snd_restart)
  * hunk users (according to `CL_StartHunkUsers`):
    * renderer
    * sound
    * UI
* See SV_Map_f for level loading code (esp. SV_SpawnServer -> G_InitGame, CG_MapChange)
* Gameloop in Com_Frame
  * Cbuf_Execute
  * Com_EventLoop
  * SV_Frame
    * G_RunFrame
  * CL_Frame
    * SCR_UpdateScreen
      * SCR_DrawScreenField // main draw function
        * CG_DrawActiveFrame // also handles audio
      * RE_EndFrame
    * S_Update

## Things to keep

* Sys_ (OS-specifics)
* Com_ (rng, events, zone & hunk memory, command line, journaling (?))
* Cvar_
* Cbuf_ = Command Buffer (input queue)
* Cmd_ = Console commands
* FS_
* SE_ = StringEd (localisation)
* CL_ = Client (partial: keybinds, console, UI connector)
* S_ = Sound
* FX_ = Effects
* R_ = Renderer
* G2_
* NET_ (low level network wrappers)
* CM_ (collision)

## Adjust/rewrite

* LAN_
* CL_ (partial: networking)
* SV_ (?)

## Unneeded

* CIN_ (.roq videos)
* ICARUS_

## Unwanted

* G_
* CG_

## Recording

* you'll be able to record alternate input sequences for each player
* optionally show past and future positions of characters
* optionally show multiple recorded paths of other character simultaneously, to preview interaction with each?
  * can't show interaction of each combination of recordings, too many
  * alternatively/additionally just allow quickly switching between different character paths (mouse wheel?)
