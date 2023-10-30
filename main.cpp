#include <iostream>
#include <unordered_map>
#include <string>

class Logger {
    std::unordered_map<void*, std::string> m_loggingMetadata;
    std::string m_init_log = "LOG OUTPUT:";
    std::string m_log;
    
    template <typename T>
    class TrackedFunctor {
        T m_function;
        Logger* m_parentLogger;
    
    public:
        TrackedFunctor (T function, Logger* logger) {
            m_function = function;
            m_parentLogger = logger;
        }
        
        template <typename... P>
        auto operator() (P... v) {
            return m_parentLogger->run(m_function, v...);
        }
    };
    
    template <typename T1, typename... T>
    void add_args(T1 v1, T... v) {
        m_log += std::to_string(v1) + ", ";
        add_args(v...);
    }
    
    template <typename T1>
    void add_args(T1 v1) {
        m_log += std::to_string(v1);
    }

	void add_args() {}
    
public:
    template <typename T>
    void attach_function(T function, std::string contents) {
        m_loggingMetadata[(void*)function] = contents;
    }
    
    
    template <typename T, typename... P>
    auto run(T function, P... args) {
        auto output = function(args...);
        m_log += "\n---\n" + m_loggingMetadata[(void*)function] + "\n\tInput: ";
        add_args(args...);
        m_log += "\n\tOutput: " + std::to_string(output);
        return output;
    }
    
    template <typename T>
    auto get_tracked_function(T function) {
        return TrackedFunctor<T> (function, this);
    }
    
    std::string get_log() const {
        return m_log;
    }
    
    void clear_log() {
        m_log = m_init_log;
    }
    
    Logger() {
        clear_log();
    }
};



// Define functions as normal 
int add(int a, int b) {
    return a+b;
}

int square(int a) {
    return a*a;
}

int main() {
	// TODO: Do testing for erroneous values such as no metadata being passed in and throw errors / remove undefined behaviour
	// NOTE: Check what happens to tracked functions when logger leaves scope / is deallocated
	// FIXME: Only works for built in types (I think, untested) because of the use of std::to_string
	// TODO: Add inline variable formatting to "attach function" metadata

    Logger logger;
	// Add text to be printed in the logger
    logger.attach_function(add, "Adding X and Y");
    logger.attach_function(square, "Squaring X");

	// Using the Logger::run function
	// Outputs can be taken as usual
	std::cout << "OUTPUTS:\n";
    std::cout << logger.run(add, 1, 2) << "\n";
    std::cout << logger.run(square, 2) << "\n\n\n";
    
	// Print the log
    std::cout << logger.get_log() << std::endl;
    
	// Clear the log
    logger.clear_log();
	// Return a new tracked function which will automatically add to the log when called
    auto addTracked = logger.get_tracked_function(add);
	// Call the function as usual
	// Outputs can be taken as usual
    std::cout << "\n\n\nOUTPUTS:\n" << addTracked(1, 2);

	// Using this functions can be composed inline as usual
	auto squareTracked = logger.get_tracked_function(square);
	std::cout << "\n" << addTracked(squareTracked(2), 1);

	// Print the log
    std::cout << "\n\n\n" << logger.get_log() << std::endl;
    
    return 0;
}