# Particles simulator
//TODO: some description

Coordinate system used here is from `-1.0` to `1.0` in both `x` and `y` axes. Origin is in the middle of the board; board is square even if OS windows is not.
## How to run
Use `run.bat` script. In source code it is in `scripts/` folder, in release zip it's in root directory. You can pass command line options, described below.
### Command line options
- `--num` (default: `10`) how many particles to generate
- `--speedFrom` (default: `-0.5`) lower bound of range in which each component of speed vector, for each particle, will be randomly selected. Valid values are from `-2.0` to `2.0`
- `--speedTo` (default: `0.5`) upper bound of thing described above. Valid values are smaller or equal to `2.0` and greater than `--speedFrom`.
- `--radiusFrom` (default: `0.025`) lower bound of range in which radius, for each particle, will be randomly selected. Valid values are from `0.025` to `0.5`
- `--radiusTo` (default: `0.2`) upper bound of thing described above. Valid values are smaller or equal to `0.5` and greater than `--radiusFrom`.
