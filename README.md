# [ 2020 - 01 - 21 ]  Updated Project & Spine2D

## Added Tactical Unit Class
 First, New add 'TacticalDoll' class and explain the code flow

### 1. AK12
 'Aliance_Griffon' type of taticdoll.

### 2. PKP
 'Aliance_Griffon' type of taticdoll.

### 3. GuA91
 'Aliance_Griffon' type of taticdoll.


## Changed Class
 Second, There are classes that change, delete, or create new classes as they are ported.


### 1. VisualBar Delete
 That class can be replaced by one function.


### 2. IOPtacDoll / SNVtacDoll Delete & Merge TaticDoll
 I had a lot of trouble! I thought this inheritance was too complicated.
So we transformed them into units with one parent class, and there were changes in the internal functions.


### 3. spineBase / spineMotion Added instead of Motion / MotionManager Class
 Now that we're able to use spine2D, we deleted the old class.
 

### 4. MemoryPool Class Added
 This newly created class is intended to prevent framedrops on frequent creation and deletion of memory.
 
 
### 5. Player singleton delete
 The Player class was originally a singleton, but with this task, it is bundled with the BattleData class.
Also, by managing all the tactical dolls used in battle as a vector, it is not as cumbersome as before, 
and there is no need to deepen the exception handling.
