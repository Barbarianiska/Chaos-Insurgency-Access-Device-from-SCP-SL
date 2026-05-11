// Start title and some lines for display output


constexpr int SNAKE_X = 51; // Start title char width
constexpr int SNAKE_Y = 8; // Start title char height

const char SNAKE[SNAKE_Y][SNAKE_X] = 
{
    " $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$\\   $$\\ $$$$$$$$\\ ",
    "$$  __$$\\ $$$\\  $$ |$$  __$$\\ $$ | $$  |$$  _____|", 
    "$$ /  \\__|$$$$\\ $$ |$$ /  $$ |$$ |$$  / $$ |      ",
    "\\$$$$$$\\  $$ $$\\$$ |$$$$$$$$ |$$$$$  /  $$$$$\\    ",
    " \\____$$\\ $$ \\$$$$ |$$  __$$ |$$  $$<   $$  __|   ",
    "$$\\   $$ |$$ |\\$$$ |$$ |  $$ |$$ |\\$$\\  $$ |      ",
    "\\$$$$$$  |$$ | \\$$ |$$ |  $$ |$$ | \\$$\\ $$$$$$$$\\ ",
    " \\______/ \\__|  \\__|\\__|  \\__|\\__|  \\__|\\________|"
};

const char BOOTING_UP[SNAKE_X] = "EMERGENCY STRESS-MITIGATION SOFTWARE BOOTING UP...";