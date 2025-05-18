# CircularLogging

CircularLogging is a Qt-based C++ application that implements a flexible circular logging system. It supports time-based log rotation (Daily, Hourly, Minutely, Secondly) and manages log files to maintain a configurable maximum number of files. The logger is configured via a JSON file and is suitable for applications requiring robust logging with automatic file management.

## Features
- **Configurable Log Rotation**: Supports `Daily`, `Hourly`, `Minutely`, and `Secondly` log types, with customizable frequency.
- **File Management**: Automatically deletes older log files when the maximum file limit is reached.
- **JSON Configuration**: Configure log type, maximum files, frequency, and log directory via a `config.json` file.
- **Qt Integration**: Built with Qt 6.9.0 for cross-platform compatibility.
- **Unit Tests**: Includes a test suite using Qt Test to validate logger functionality.

## Prerequisites
To build and run CircularLogging, you need:
- **Qt**: Version 6.9.0 or later (with Qt Core and Qt Test modules).
- **CMake**: Version 3.5 or later.
- **Compiler**: A C++17-compatible compiler (e.g., MinGW 64-bit, MSVC).
- **Git**: For cloning the repository (optional).

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/CircularLogging.git
   cd CircularLogging
   ```

2. **Create a Build Directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the Project**:
   ```bash
   cmake --build . --config Release
   ```

## Usage
1. **Prepare a Configuration File**:
   Create a `config.json` file in the build directory (e.g., `build/`) with the following structure:
   ```json
   {
       "logType": "Secondly",
       "maxFiles": 5,
       "frequency": 1,
       "logDir": "logs"
   }
   ```
   - `logType`: `Daily`, `Hourly`, `Minutely`, or `Secondly`.
   - `maxFiles`: Maximum number of log files to keep (e.g., 5).
   - `frequency`: Interval multiplier for log rotation (e.g., 1 for every unit of `logType`).
   - `logDir`: Directory for log files (relative or absolute path, e.g., `logs`).

2. **Run the Application**:
   ```bash
   ./CircularLogging
   ```
   The application logs messages every 2 seconds (e.g., "Test log message #1") to files in the `logs` directory. Log files are named based on `logType`, e.g., `log_2025-05-18_12-34-56.txt` for `Secondly`.

3. **Verify Log Files**:
   Check the `logs` directory for files containing timestamped messages, e.g.:
   ```
   2025-05-18 12:34:56 UTC: Test log message #1
   2025-05-18 12:34:58 UTC: Test log message #2
   ```

## Testing
The project includes unit tests to validate the `CircularLogger` class, implemented using Qt Test.

1. **Build the Test Executable**:
   Ensure the project is built (as above).

2. **Run the Tests**:
   ```bash
   ./test_CircularLogger
   ```
   Tests cover:
   - Loading configuration from `config.json`.
   - Logging messages to files.
   - Log file rotation for different `logType` settings.
   - Maximum file limit enforcement.

3. **Test Output**:
   Expect output like:
   ```
   ********* Start testing of TestCircularLogger *********
   PASS   : TestCircularLogger::initTestCase()
   PASS   : TestCircularLogger::testLoadConfig()
   PASS   : TestCircularLogger::testLogDaily()
   PASS   : TestCircularLogger::testLogRotation()
   PASS   : TestCircularLogger::testMaxFiles()
   PASS   : TestCircularLogger::cleanupTestCase()
   Totals: 6 passed, 0 failed, 0 skipped, 0 blacklisted
   ********* Finished testing of TestCircularLogger *********
   ```

## Project Structure
```
CircularLogging/
├── CMakeLists.txt        # CMake configuration file
├── main.cpp              # Main application entry point
├── CircularLogger.h      # Logger class header
├── CircularLogger.cpp    # Logger class implementation
├── test_CircularLogger.cpp # Unit tests for CircularLogger
├── config.json           # Sample configuration file
├── .gitignore            # Git ignore rules
├── README.md             # Project documentation
```

## Troubleshooting
- **Config File Missing**: Ensure `config.json` exists in the build directory. See the sample above.
- **Log Files Not Created**: Verify that `logDir` is writable and `config.json` is valid. Check console for `qWarning()` messages.
- **Test Failures**: Run tests with verbose output (`./test_CircularLogger -v`) to debug failures.
- **Build Issues**: Ensure Qt 6.9.0 and CMake are correctly installed. Check CMake output for errors.

## Contributing
Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your-feature`).
3. Commit your changes (`git commit -m "Add your feature"`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Open a pull request.

Please include tests for new features and follow the existing coding style.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact
For questions or feedback, open an issue on GitHub or contact [your email or preferred contact method].
