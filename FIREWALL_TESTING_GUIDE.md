# Firewall Testing Guide

## Overview
The Firewall ability has been implemented with the following functionality:
- Reveals opponent links when they pass through a firewall
- Downloads virus links immediately when they pass through a firewall
- Applies firewall effects before battles when a link is stationed on a firewall
- Firewalls are displayed as 'm' for player 1 and 'w' for player 2

## Test Files Provided

### 1. `test_firewall_simple.txt`
Basic functionality test covering:
- Placing firewalls
- Moving opponent links through firewalls (reveal effect)
- Moving virus links through firewalls (download effect)
- Moving own links through own firewalls (no effect)
- Battling on firewalls

### 2. `test_firewall.txt`
Comprehensive test covering:
- All basic functionality
- Edge cases
- Multiple firewalls
- Interaction with other abilities
- Server port interactions

### 3. `test_firewall_abilities.txt`
Tests firewall with different ability configurations:
- Multiple firewalls
- Interaction with LinkBoost
- Interaction with Download and Scan abilities

## Key Test Scenarios

### 1. Basic Firewall Placement
```
ability 2 3 4
```
- Should place firewall at position (3,4)
- Should display as 'w' (player 2 firewall)
- Should fail if placed on server port
- Should fail if placed on non-empty cell

### 2. Link Revelation
```
move A down  # Move opponent link through firewall
```
- Should reveal the opponent link
- Should show the link's type and strength
- Should not affect own links

### 3. Virus Download
```
move D down  # Move virus link through firewall
```
- Should reveal the virus link
- Should immediately download the virus
- Should increment the virus owner's virus count
- Should remove the link from the board

### 4. Battle on Firewall
```
move C up    # Attack link stationed on firewall
```
- Should apply firewall effects before battle
- Should reveal attacking link
- Should download attacking virus if applicable
- Should proceed with normal battle if no download

### 5. Multiple Firewalls
```
ability 2 3 4
ability 2 4 4
ability 2 5 4
```
- Should allow multiple firewalls
- Should apply effects at each firewall
- Should handle multiple downloads correctly

## Expected Behavior

### Firewall Effects
- **Opponent Links**: Always revealed when passing through firewall
- **Virus Links**: Revealed AND downloaded when passing through firewall
- **Data Links**: Only revealed when passing through firewall
- **Own Links**: No effect when passing through own firewall

### Visual Indicators
- Firewalls display as 'm' (player 1) or 'w' (player 2)
- Revealed links show their type and strength
- Downloaded links are removed from the board

### Download Mechanics
- Virus downloads count toward the virus owner's total
- Downloaded viruses can trigger victory/loss conditions
- Download happens immediately when virus passes through firewall

## Running Tests

### Basic Test
```bash
./raiinet < test_firewall_simple.txt
```

### Comprehensive Test
```bash
./raiinet < test_firewall.txt
```

### Test with Custom Abilities
```bash
./raiinet -ability1 "FFDDL" -ability2 "FFDDL" < test_firewall_abilities.txt
```

## Common Issues to Check

1. **Firewall not revealing links**: Check if opponent links are being revealed
2. **Virus not downloading**: Check if virus links are being downloaded immediately
3. **Own links affected**: Check if own links are unaffected by own firewalls
4. **Battle effects**: Check if firewall effects apply before battles
5. **Visual display**: Check if firewalls display correctly as 'm'/'w'
6. **Download counting**: Check if virus downloads count toward correct player

## Debugging Tips

- Use `board` command frequently to check board state
- Use `abilities` command to see available abilities
- Check player download counts after virus downloads
- Verify firewall positions are correct
- Ensure links are being revealed properly 