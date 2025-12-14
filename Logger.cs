using System;
using System.IO;

namespace uc_online
{
    public class Logger
    {
        private string _logFilePath;
        private bool _loggingEnabled;
        private object _lockObject = new object();

        public Logger(string logFilePath, bool enableLogging)
        {
            _logFilePath = logFilePath;
            _loggingEnabled = enableLogging;

            if (_loggingEnabled)
            {
                Log("Logger initialized");
            }
        }

        public void SetLoggingEnabled(bool enabled)
        {
            _loggingEnabled = enabled;
            Log($"Logging {(enabled ? "enabled" : "disabled")}");
        }

        public bool IsLoggingEnabled()
        {
            return _loggingEnabled;
        }

        public void Log(string message)
        {
            if (!_loggingEnabled) return;

            try
            {
                string logMessage = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss} [INFO] {message}";

                lock (_lockObject)
                {
                    File.AppendAllText(_logFilePath, logMessage + Environment.NewLine);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Logging error: {ex.Message}");
            }
        }

        public void LogWarning(string message)
        {
            if (!_loggingEnabled) return;

            try
            {
                string logMessage = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss} [WARNING] {message}";

                lock (_lockObject)
                {
                    File.AppendAllText(_logFilePath, logMessage + Environment.NewLine);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Logging error: {ex.Message}");
            }
        }

        public void LogError(string message)
        {
            if (!_loggingEnabled) return;

            try
            {
                string logMessage = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss} [ERROR] {message}";

                lock (_lockObject)
                {
                    File.AppendAllText(_logFilePath, logMessage + Environment.NewLine);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Logging error: {ex.Message}");
            }
        }

        public void LogException(Exception ex, string context = "")
        {
            if (!_loggingEnabled) return;

            try
            {
                string logMessage = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss} [EXCEPTION] {context}: {ex.GetType().Name} - {ex.Message}{Environment.NewLine}{ex.StackTrace}";

                lock (_lockObject)
                {
                    File.AppendAllText(_logFilePath, logMessage + Environment.NewLine);
                }
            }
            catch (Exception logEx)
            {
                Console.WriteLine($"Logging error: {logEx.Message}");
            }
        }

        public void ClearLog()
        {
            if (!_loggingEnabled) return;

            try
            {
                lock (_lockObject)
                {
                    File.WriteAllText(_logFilePath, $"uc-online Log - {DateTime.Now:yyyy-MM-dd HH:mm:ss}{Environment.NewLine}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error clearing log: {ex.Message}");
            }
        }
    }
}