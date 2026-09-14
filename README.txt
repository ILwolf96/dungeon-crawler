Hey there, so....
This is my Final Project for Tiltan, I might gone bit ambitious,
the message at the end or the brief of "Ship something you're proud of."
somewhat also pushed me to do so.
I didn't Manage to put ALL what I wanted, but I'm Proud of what I made.
anw, I will stop rambling, about this.
and move on to write about the Project.

## Dungeon Crawler ##
it's C++ dungeon crawler built with raylib.
The game loads the player, enemies, gear, and dungeon layout from configuration files and data files, and it also supports both INI and JSON loading.


--------------------------------------------------------------------------------------

## How to Build ##
1. Open the project folder in Visual Studio.
2. Select the **x64-Debug** CMake configuration.
3. Build the project with **Build -> Build All**. (sometimes need to close and open visual studio and it will fix it, or to press Rebuild All)
4. Run "dungeon-crawler.exe".

The executable is generated inside the CMake build output folder.


--------------------------------------------------------------------------------------

## How to Play ##
I provided Instructions all over the game, which you can acess to using
TAB - Which Will Open Instructions, depending on the State of the game.

When You Run The Program, it will lauch and show you a Launch Menu,
That I created.

at the very Bottom you can see the aviable actions.

1 - Launch, will Launch the Game with the Current settings.
2 - Settings, Will Open The Setting you can Change, which are:
 A - Load Method, You Can Change the load of Data, to use INI or JSON.
 B - Change Map, There Two Maps:
 1) Dungeon -  Which is the Game,
 2) Zoo - A Small Debug and Testing Area for all the Features in the Game,
     With Some that are only exist in Zoo, Such as:
     a) W which increase the Tier of the Weapon The Player Owns.
     b) w which decrease the Tier of the Weapon The Player Owns.
     c) A which increase the Tier of the Armor The Player Owns.
     d) a which decrease the Tier of the Armor The Player Owns.
     e) X which increase (By Adding) the Tier of the Accessory The Player Owns.
     f) x which decrease (by Removing) the Tier of the Accessory The Player Owns.
     g) R which Respawns all Entites (chests included, yes, they are mimics).
     h) H which Heals The Player To Full.

Use "WASD" or the "Arrow Keys" to move around the dungeon.
Use "Q / E" to turn the camara of the Player.
(you always move reletive to the dungeon, not where you are facing,
it sucks abit, I know)

a General Combat encounter With an enemy,
Starts when you walk over the Tile the enemy (or Chest) is in.

During Combat The Player Will have the following Options:

A) F1 / 1 - Attack
B) I / 2 - Open Inventory
C) E / 3 - Escape Combat

if they Press I to Open The Inventory, They will be givin:
a) 1 - Use Health Potion - Heals X (3 atm) Hp)
b) 2 - Use Rage Potion - Increases the Player Damage
c) Close Inventory
d) Tab Show Inventory Instruction - that provide more info gear and invetory.

Into more Nerdy details:
#Stats#

HP
Health of the entity, When HP reaches 0, the entity is defeated.

ATK
Number of attacks made during one attack action.

Precision
The minimum D6 result required for an attack to hit.

Strength
Offensive power used against the target's Toughness.

Toughness
Determines how difficult a target is to wound.

Defense
The minimum D6 result required to block incoming damage.

Damage
Amount of HP removed when an attack successfully deals damage.

##Combat

Combat is turn-based.

The player selects an action, the action is resolved, and then the enemy gets its turn when required.

The main combat actions are:

Attack
Use the character's ATK to perform attacks.

Inventory
Use a potion. Using a potion ends the player's turn, allowing the enemy to attack.

Escape
Leave the current combat without defeating the enemy.

# Attack Math#
Each individual attack uses the following sequence:

1. Precision Roll
Roll a D6.

If:
`Roll >= Precision`
the attack hits.

If:
`Roll < Precision`
the attack misses.

2. Wound Roll
After a successful Precision Roll, compare Strength with the target's Toughness.

The required Wound Target is:
Toughness <= 0 -> Target 2
Strength >= Toughness x 2 -> Target 2
Strength > Toughness -> Target 3
Strength = Toughness -> Target 4
Strength x 2 <= Toughness -> Target 6
Otherwise -> Target 5

Roll another D6.

If:
Roll >= Wound Target
the attack successfully wounds the target.

3. Defense Roll
The defender rolls a D6.

If:
`Roll >= Defense`
the attack deals 0 damage.

If:
Roll < Defense
the attack deals its Damage value.

#Multiple Attacks#
ATK determines how many individual attacks are made.

For example:
ATK = 2
means the character performs two complete attack sequences.
Each attack gets its own Precision, Wound, and Defense rolls.

#Damage Bonuses#
Damage can be increased by temporary combat bonuses.
A Rage Potion adds its configured Damage Bonus to the player's damage for the current combat.
Damage bonuses stack when multiple Rage Potions are used.

#Defeat#
A entity is defeated when:
HP <= 0

(note A defeated enemy remains on the map as a defeated enemy.)
The player is defeated when their HP reaches 0.

#Combat Sum#
A short Combat Summary of An attack can be represented as:
ATK -> Precision -> Wound -> Defense -> Damage

A successful attack must first hit, then wound, and finally get through the defender's Defense before damage is applied.


the Idea is to Use potions when needed, collect better gear to defeat stronger enemies, and explore the dungeon.

The goal is to explore the dungeon, defeat enemies, collect loot, to defeat stronger enemies and reach the Portal to escape.


--------------------------------------------------------------------------------------

##Design Note##

The Overall Project is built around your suggestion in the presetion of
"Designers edit text; programmers keep the loop
Change the file, not the code: redraw the map, rebalance the dragon, or add a second sword in dungeon.ini — then relaunch. 
No recompiling."


While The dungeon is designed around gradual progression.
The player starts with basic equipment and encounters weaker enemies first, allowing them to collect better gear and resources before moving deeper into the dungeon.
Multiple rooms, branches, enemies, and chests give the player opportunities to explore, prepare, retreat from difficult fights, and try again.

The final areas contain the strongest enemies and lead to the Portal, giving the level a clear sense of progression from the entrance to the final escape.

(Which I will be Honst I cannot Confirm if the Game is Balanced well,
cuz I went crazy with the Scope)

I took big Inspration from Warhammer Table Top Games,
and tired to learn tabletop rulesin the background, 
while Researching build with raylib, 
while I wrote the code, Draw the Art and etc.
