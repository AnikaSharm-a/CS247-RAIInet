# AreaScan Ability Testing Guide

## Overview
The AreaScan ability is a "Scan-lite" ability that reveals all enemy links in the 8 surrounding squares (3x3 area) around one of your own links. This provides broader reconnaissance compared to the single-target Scan ability.

## AreaScan Ability Details

### **Input Format**
- **Input**: Link ID of one of your own links
- **Effect**: Reveals all opponent links in the 3x3 area around your specified link
- **Example**: `ability 1 d` - scans the 3x3 area around your link 'd'

### **Key Features**
- ✅ **Link ID Input**: Uses link ID like LinkBoost (must be your own link)
- ✅ **3x3 Scan Area**: Reveals all opponent links in 8 surrounding squares + center
- ✅ **Own Link Required**: Can only scan around your own links
- ✅ **Opponent Links Only**: Only reveals opponent links, not own links
- ✅ **Boundary Handling**: Properly handles board edges and corners
- ✅ **Validation**: Requires at least one opponent link in scan area

## Scan Area Coverage

The AreaScan covers a 3x3 grid centered on your specified link:

```
For link 'd' at position (1,3):
[0,2] [0,3] [0,4]
[1,2] [1,3] [1,4]  <- Your link 'd' at center
[2,2] [2,3] [2,4]
```

**Boundary Handling:**
- At edges: Only scans valid board squares
- At corners: Only scans the 4 squares that exist
- Example at link 'a' (0,0): Only scans [0,0], [0,1], [1,0], [1,1]

## Test Files Provided

### 1. `test_areascan.txt`
Basic AreaScan functionality test covering:
- Scanning around own links in different positions
- Edge boundary handling
- Corner boundary handling
- Error cases (no opponent links, wrong link ownership, invalid link ID)
- Comparison with regular Scan

### 2. `test_all_abilities_with_areascan.txt`
Comprehensive test of all abilities including AreaScan:
- All abilities with their respective input formats
- AreaScan edge cases
- Integration with other abilities
- Error handling

## Key Test Scenarios

### 1. **Basic AreaScan Testing**
```
ability 1 d    # Should reveal multiple opponent links around link 'd'
ability 1 a    # Should reveal opponent links around link 'a'
```

### 2. **Boundary Testing**
```
ability 1 a    # Corner scan around link 'a' (4 squares max)
ability 1 b    # Edge scan around link 'b'
ability 1 c    # Another area scan
```

### 3. **Error Handling**
```
ability 1 c    # Should fail if no opponent links around link 'c'
ability 1 A    # Should fail - cannot scan around opponent link
ability 1 X    # Should fail - invalid link ID
```

### 4. **Comparison with Regular Scan**
```
ability 1 A    # Regular Scan - single target
ability 1 d    # AreaScan - multiple targets around your link
```

## Expected Behavior

### AreaScan Effects
- **Multi-Target Revelation**: Reveals all opponent links in 3x3 area around your link
- **Own Link Protection**: Does not reveal own links
- **Own Link Requirement**: Can only scan around your own links
- **Boundary Respect**: Only scans valid board coordinates
- **Validation**: Requires at least one opponent link to succeed

### Error Handling
- **No Opponent Links**: Clear error if no opponent links in scan area
- **Wrong Link Ownership**: Clear error if trying to scan around opponent link
- **Invalid Link ID**: Clear error for non-existent link ID
- **Missing Arguments**: Clear error for missing link ID

### Visual Indicators
- **Text Display**: Revealed links show their type and strength
- **Graphic Display**: Revealed links show their actual information
- **Player Info**: Revealed links appear in opponent's link list

## Running Tests

### Basic AreaScan Test
```bash
./raiinet -ability1 "LFDSA" -ability2 "LFDSA" < test_areascan.txt
```

### Comprehensive Ability Test
```bash
./raiinet -ability1 "LFDSA" -ability2 "LFDSA" < test_all_abilities_with_areascan.txt
```

### Manual Testing
```bash
./raiinet -ability1 "LFDSA" -ability2 "LFDSA"
# Then use commands like:
# ability 1 d    # AreaScan around your link 'd'
# ability 1 a    # AreaScan around your link 'a'
# ability 1 A    # Regular Scan for comparison
```

## Common Issues to Check

1. **Scan Area Coverage**: Ensure all 9 squares (or valid subset) are scanned
2. **Boundary Handling**: Verify edge and corner scans work correctly
3. **Opponent Link Filtering**: Confirm only opponent links are revealed
4. **Own Link Protection**: Verify own links remain hidden
5. **Error Messages**: Ensure clear error messages for invalid usage
6. **Integration**: Verify AreaScan works with other abilities

## Debugging Tips

- Use `board` command frequently to check board state
- Use `abilities` command to see available abilities
- Check that revealed links show correct information
- Verify boundary scans only cover valid squares
- Test with different board positions to ensure coverage
- Compare with regular Scan to understand the difference

## Strategic Usage

### Reconnaissance Strategy
- Use AreaScan to quickly assess opponent positions
- Scan strategic areas before making moves
- Use in combination with other abilities for maximum information

### Positioning Strategy
- Place scans to maximize opponent link coverage
- Use edge scans to check opponent deployment
- Scan center areas to understand opponent strategy

### Comparison with Regular Scan
- **AreaScan**: Broader coverage, less precise
- **Regular Scan**: Single target, more precise
- **Strategic Choice**: Choose based on information needs

## Technical Implementation

### Key Methods
- **`AreaScan::use()`**: Main implementation in `areascan.cc`
- **Link ID Parsing**: Uses existing link ID parsing in `game.cc` like LinkBoost
- **Ownership Validation**: Verifies the link belongs to the current player
- **Boundary Handling**: Uses `max(0, row-1)` and `min(7, row+1)` for bounds
- **Link Filtering**: Only reveals links where `link->getOwner() != player`

### Integration Points
- **AbilityFactory**: Added 'A' code and "AreaScan" name
- **Game Logic**: Uses link ID parsing like LinkBoost (must be own link)
- **Error Handling**: Centralized in controller with other abilities