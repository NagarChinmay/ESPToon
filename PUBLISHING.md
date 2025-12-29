# Publishing Guide for ESPToon

This guide helps you publish the ESPToon library to Arduino Library Manager and PlatformIO Registry.

## Prerequisites

- [x] Git repository created
- [x] Library code complete
- [x] Examples tested
- [x] Documentation ready
- [x] License file (MIT)

## Publishing to Arduino Library Manager

### Step 1: Prepare GitHub Repository

1. Create a new repository on GitHub:
   ```bash
   cd /home/chinmay-nagar/Documents/ESPToon
   git init
   git add .
   git commit -m "Initial commit: ESPToon v1.0.0"
   git branch -M main
   git remote add origin https://github.com/nagarChinmay/ESPToon.git
   git push -u origin main
   ```

2. Create a release tag:
   ```bash
   git tag -a v1.0.0 -m "ESPToon v1.0.0 - Initial release"
   git push origin v1.0.0
   ```

### Step 2: Verify Library Compliance

Arduino Library Manager requirements:
- ✅ `library.properties` file exists
- ✅ Valid `name`, `version`, `author`, `maintainer` fields
- ✅ `sentence` and `paragraph` descriptions
- ✅ `category` is valid (Data Storage)
- ✅ `architectures` specified (esp32, esp8266)
- ✅ `src/` folder contains all source files
- ✅ `examples/` folder contains .ino sketches
- ✅ `LICENSE` file exists
- ✅ Valid semver version number (1.0.0)

### Step 3: Submit to Arduino Library Manager

1. Go to: https://github.com/arduino/library-registry
2. Add your repository URL to the list
3. Create a Pull Request
4. Wait for automated checks to pass
5. Arduino team will review and merge

**Indexing time:** Usually 1-7 days after merge

### Step 4: Verify Installation

After published:
```
Arduino IDE → Sketch → Include Library → Manage Libraries
Search for "ESPToon"
```

## Publishing to PlatformIO Registry

### Step 1: Verify PlatformIO Compliance

- ✅ `library.json` file exists
- ✅ Valid `name`, `version`, `description` fields
- ✅ `keywords` array populated
- ✅ `authors` array with contact info
- ✅ `repository.url` points to Git repo
- ✅ `frameworks` set to "arduino"
- ✅ `platforms` includes "espressif32", "espressif8266"
- ✅ `dependencies` lists ArduinoJson

### Step 2: Register Library

Option A: Automatic (Recommended)
```bash
pio package publish
```

Option B: Manual Registry
1. Go to: https://platformio.org/
2. Sign in / Create account
3. Go to "Libraries" → "Submit Library"
4. Enter GitHub repository URL
5. Submit

### Step 3: Verify Installation

After published:
```ini
# platformio.ini
lib_deps =
    ESPToon
```

Or install via CLI:
```bash
pio lib install ESPToon
```

## Post-Publishing Checklist

- [ ] Test installation from Arduino Library Manager
- [ ] Test installation from PlatformIO
- [ ] Verify examples work on fresh install
- [ ] Update repository README with installation badges
- [ ] Announce release (forums, social media)
- [ ] Monitor GitHub issues for bug reports

## Version Updates

When releasing updates:

1. Update version in both files:
   - `library.properties` → `version=1.0.1`
   - `library.json` → `"version": "1.0.1"`

2. Create git tag:
   ```bash
   git tag -a v1.0.1 -m "Version 1.0.1 - Bug fixes"
   git push origin v1.0.1
   ```

3. Both registries will auto-detect the new release

## Semver Guidelines

- **Major (1.0.0 → 2.0.0)**: Breaking API changes
- **Minor (1.0.0 → 1.1.0)**: New features, backward compatible
- **Patch (1.0.0 → 1.0.1)**: Bug fixes only

## Support & Issues

- **GitHub Issues**: https://github.com/nagarChinmay/ESPToon/issues
- **Email**: chinmay@chipnexa.in
- **Documentation**: See README.md

## Marketing Tips

1. **Arduino Forum Post**: Share in "Exhibition / Gallery"
2. **Reddit**: r/arduino, r/esp32, r/embedded
3. **Twitter/X**: Use #arduino #esp32 #embedded #iot
4. **Hackster.io**: Create project tutorial
5. **YouTube**: Demo video showing key features

## License

This library is released under MIT License. Contributors must agree to the same license.

## Credits

**Author:** Chinmay Nagar
**Organization:** Chipnexa
**Year:** 2025
