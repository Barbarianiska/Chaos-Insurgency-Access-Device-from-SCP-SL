// Map size constants
constexpr int MX = 17; // Map length
constexpr int MY = 15; // Map width
constexpr int MS = MX * MY; // Map size

unsigned long PM = 0; // Previous milliseconds keyboard timer, ms
unsigned long CM; // Current milliseconds keyboard timer, ms
constexpr uint16_t STI = 200; // Start time interval, ms
uint16_t TI; // Time interval, ms


// Directions of head
enum Direction
{ 
    UP = -static_cast<int>(MX), 
    DOWN = static_cast<int>(MX), 
    LEFT = -1,
    RIGHT = 1,
    NONE = 0
};


// Snake constants
constexpr int SSL = 5; // Start snake length
constexpr int HSP = (1 + (MX - 1)) * (1 + (MY - 1)) / 2; // Head start position
constexpr int TSP = HSP - SSL + 1; // Tail start position
constexpr Direction HSD = RIGHT; // Head start direction
constexpr Direction TSD = HSD; // Tail start direction


// Snake parameters
Direction HCD; // Current head direction
Direction TCD; // Current tail direction
bool RST = false; // Start and restart flag
Direction DA[MS]; // Directions array
int SL; // Snake length
int HP; // Head position
int TP; // Tail position
bool MP[MS]; // 1D map array