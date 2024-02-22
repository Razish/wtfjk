# WTFJK PLANS

## FIXME

- roshinvasion starting when enemies are dead breaks the dead animation (t-pose)
- drunk mode breaks skipping cinematics (timescale != 1.0?)
- inclement weather doesn't reset the constantwind direction (shielding animation always plays 😮‍💨)
- inclement weather (and probably others) doesn't reset on quickload (though another mutator is active)
- when giants mode ends, enemies are halfway stuck in the floor
- giants: bboxes and saber/projectile collisions are not scaled 😭
- second person: NPCs should constantly walk toward player
- bouncysandcreatures: player sounds are not reset when it ends
- bouncysandcreatures: if it starts during a cutscene, you exit the cutscene with the player model and regular orientation
- bouncysandcreatures: if you load a savegame that has BSC active, you will spawn at full-size and regular orientation - need a Resume() func?
- bouncysandcreatures: sometimes your first movement will play a (death?) animation 😕

## TODO

- think of more mutators
- port rd-badcandy for visual effects (glsl)
  - noir
  - acid trip
- obtain/create assets for mutators
  - tinnitus sound
  - tyrant stomp sound
  - models
    - dickbutt (ashura)
  - NPCs
- add a helper func to spawn an NPC by name, not spawn func

## MUTATOR IDEAS

- paranoia: bunch of ghost enemies spawn and float around
  - only render force sight shell?
- enemy blasters shoot out mouse droids that inject drugs into you
- spider popup, crawls across the screen. but only rarely
- random black hair strand appears on the screen
- a meta mutator that transforms the game into autismjk?
  - dickbutts randomly spawn in the map, become deformed and vanish
  - CoD hit markers
- mine monster snarks
- floor is lava - check ground trace over the last 10s, gradually increase height
- sharknadoes
