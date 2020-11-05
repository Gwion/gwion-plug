enum libevdev_log_priority {
	LIBEVDEV_LOG_ERROR = 10,	/**< critical errors and application bugs */
	LIBEVDEV_LOG_INFO  = 20,	/**< informational messages */
	LIBEVDEV_LOG_DEBUG = 30		/**< debug information */
};
typedef void (*libevdev_device_log_func_t)(const struct libevdev *dev,
					   enum libevdev_log_priority priority,
					   void *data,
					   const char *file, int line,
					   const char *func,
					   const char *format, va_list args)
	LIBEVDEV_ATTRIBUTE_PRINTF(7, 0);
void libevdev_set_device_log_function(struct libevdev *dev,
				      libevdev_device_log_func_t logfunc,
				      enum libevdev_log_priority priority,
				      void *data);
