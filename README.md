# logger

[Old code] [Archived]

---

Logging class which wraps a functor and logs inputs and outputs when that functor is called.

---

```cpp
Logger logger;

// Attach function to the logger instance
logger.attach_function(add, "Adding X and Y");

// Run the function indirectly through the logger instance
std::cout << ":: " << logger.run(add, 1, 2) << "\n\n";
    
// Print the log
std::cout << logger.get_log() << std::endl;
    
// Clear the log
logger.clear_log();

// Create a new tracked function which will add to the log when called
auto add_ = logger.get_tracked_function(add);
std::cout <<  ":: " << add_(3, 4) << "\n\n";

// Print the log
std::cout << logger.get_log() << std::endl;
```

```
:: 3

LOG OUTPUT:
---
Adding X and Y
	Input: 1, 2
	Output: 3
:: 7

LOG OUTPUT:
---
Adding X and Y
	Input: 3, 4
	Output: 7
```
