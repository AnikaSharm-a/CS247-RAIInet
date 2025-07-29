# Jam Ability Testing Guide

## Overview
The Jam ability allows a player to target an opponent's link and prevent it from moving for one turn. This is a powerful defensive ability that can disrupt opponent strategies.

## Jam Ability Details

### **Input Format**
- **Input**: Single opponent link ID (e.g., `ability 1 A`)
- **Effect**: Prevents the targeted opponent link from moving for one full turn cycle
- **Example**: `ability 1 A` - jams opponent link 'A'

### **Key Features**
- ✅ **Opponent Links Only**: Can only jam opponent links
- ✅ **One Turn Cycle Duration**: Jam effect lasts until the end of the opponent's turn
- ✅ **Movement Prevention**: Jammed links cannot move
- ✅ **Visual Indicators**: Jammed links display with "!" in the interface
- ✅ **Automatic Removal**: Jam effect is automatically removed after opponent's turn

## Turn-Based Jam Duration

The jam effect works as follows:
1. **Player 1 jams Player 2's link**: Link is jammed
2. **Player 1's turn ends**: Link remains jammed
3. **Player 2's turn**: Link is still jammed and cannot move
4. **Player 2's turn ends**: Link is automatically unjammed
5. **Player 1's next turn**: Link can move normally

### Example Timeline:
```
Turn 1: Player 1 jams link C
Turn 1: Player 1 moves own link (ends turn)
Turn 2: Player 2 tries to move C (fails - still jammed)
Turn 2: Player 2 moves own link (ends turn)
Turn 3: Player 1 can move C (unjammed)
```

## Test Files Provided

### 1. `test_jam.txt`
Basic Jam functionality test covering:
- Jamming opponent links
- Preventing movement of jammed links
- Error handling for invalid inputs
- Turn-based jam removal
- Interaction with other abilities

### 2. `test_all_abilities_with_jam.txt`
Comprehensive test of all abilities including Jam:
- All abilities with link ID input
- Jam interaction with other abilities
- Error handling
- Edge cases

## Key Test Scenarios

### 1. **Basic Jam Testing**
```
ability 1 A    # Should jam opponent link 'A'
move A up      # Should fail - link is jammed
```

### 2. **Turn-Based Jam Removal**
```
ability 1 A    # Jam link 'A' (Player 1's turn)
move a up      # End Player 1's turn
move A up      # Should fail - still jammed (Player 2's turn)
move A down    # End Player 2's turn
move A up      # Should work - jam removed (Player 1's turn)
```

### 3. **Own Link Protection**
```
ability 1 a    # Should fail - cannot jam own link
```

### 4. **Multiple Jammed Links**
```
ability 1 A    # Jam link 'A'
ability 1 B    # Jam link 'B'
move A up      # Should fail
move B down    # Should fail
move C right   # Should work
```

### 5. **Jam with Other Abilities**
```
ability 1 A    # Jam link 'A'
ability 1 A    # Try to scan jammed link (should work)
ability 1 A    # Try to polarize jammed link (should work)
```

## Expected Behavior

### Jam Effects
- **Movement Prevention**: Jammed links cannot move in any direction
- **Ability Interaction**: Other abilities can still target jammed links
- **Visual Display**: Jammed links show "!" indicator
- **Turn Duration**: Effect lasts until the end of the opponent's turn

### Error Handling
- **Own Links**: Cannot jam own links
- **Invalid Links**: Clear error for non-existent links
- **Missing Arguments**: Clear error for missing link ID

### Visual Indicators
- **Text Display**: Jammed links show "!" after the link ID
- **Graphic Display**: Jammed links show "!" indicator
- **Player Info**: Jammed links show "!" in link lists

## Running Tests

### Basic Jam Test
```bash
./raiinet -ability1 "LFDSPJ" -ability2 "LFDSPJ" < test_jam.txt
```

### Jam Fix Test
```bash
./raiinet -ability1 "LFDSPJ" -ability2 "LFDSPJ" < test_jam_fix.txt
```

### Comprehensive Ability Test
```bash
./raiinet -ability1 "LFDSPJ" -ability2 "LFDSPJ" < test_all_abilities_with_jam.txt
```

### Manual Testing
```bash
./raiinet -ability1 "LFDSPJ" -ability2 "LFDSPJ"
# Then use commands like:
# ability 1 A    # Jam opponent link 'A'
# move A up      # Should fail
# move a up      # End turn
# move A up      # Should still fail (opponent's turn)
# move A down    # End opponent's turn
# move A up      # Should work now
```

## Common Issues to Check

1. **Jam Application**: Ensure only opponent links can be jammed
2. **Movement Prevention**: Verify jammed links cannot move
3. **Turn Duration**: Check that jam effect persists through opponent's turn
4. **Visual Indicators**: Confirm jammed links show "!" indicator
5. **Ability Interaction**: Verify other abilities can still target jammed links
6. **Error Messages**: Ensure clear error messages for invalid usage

## Debugging Tips

- Use `board` command frequently to check board state
- Use `abilities` command to see available abilities
- Check for "!" indicators on jammed links
- Verify movement attempts on jammed links fail
- Test turn transitions to ensure jam removal timing
- Check that own links cannot be jammed
- Verify jammed links can still be targeted by other abilities

## Strategic Usage

### Defensive Strategy
- Jam opponent links that are close to your server ports
- Jam links that are about to attack your pieces
- Use jam to buy time for your own moves

### Offensive Strategy
- Jam opponent links to prevent them from blocking your path
- Use jam to isolate opponent pieces
- Combine jam with other abilities for maximum effect

### Timing Considerations
- Jam effect lasts one full turn cycle, so timing is crucial
- Consider the turn order when using jam
- Plan your moves around when jam effects will expire
- Jam can be used strategically to prevent opponent moves during their turn