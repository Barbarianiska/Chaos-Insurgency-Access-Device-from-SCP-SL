// CHAOS INSURGENCY ACCESS DEVICE

// Version 5.2
// WRITE TEXT TILL THE END


//  $$$$$$\  $$\   $$\  $$$$$$\  $$\   $$\ $$$$$$$$\ 
// $$  __$$\ $$$\  $$ |$$  __$$\ $$ | $$  |$$  _____|
// $$ /  \__|$$$$\ $$ |$$ /  $$ |$$ |$$  / $$ |      
// \$$$$$$\  $$ $$\$$ |$$$$$$$$ |$$$$$  /  $$$$$\    
//  \____$$\ $$ \$$$$ |$$  __$$ |$$  $$<   $$  __|   
// $$\   $$ |$$ |\$$$ |$$ |  $$ |$$ |\$$\  $$ |      
// \$$$$$$  |$$ | \$$ |$$ |  $$ |$$ | \$$\ $$$$$$$$\ 
//  \______/ \__|  \__|\__|  \__|\__|  \__|\________|


// ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░
// EMERGENCY STRESS-MITIGATION SOFTWARE BOOTING UP...


// COMMENTS BY ILYA


// MAIN FEATURES

// Walls in this version of snake will not have any collision. Snake moves in ring world
// Map size: 19x11. Size of a rectangle for the map (for sprites 15x15): 287x167  
// Start length of the snake (with tail and head): 5
// Start position (of head): 10x6
// Collision event: snake makes one more step, sprites of body and head overlap each other, shake disappears, appears and disappears (food stays on map). Game starts again
// Two steps before the food snake opens its mouth
// After eating, body becomes full
// When snake is approaching (2 tiles) straight to the food, mouth opens
// The game becomes faster when snake becomes bigger, in my opinion (start step time is about 500 ms)
// Font for start text and counter: VCR OSD Mono. Font size depends on display pixel size


// PROBLEM WITH FONTS

// Adafruit GFX library has non-compiled C file "fontconvert.c". I've compiled it using gcc. There was a problem with header file "ft2build.h".
// I installed it with command "pacman -S mingw-w64-x86_64-freetype". Common convert command syntax: "./fontconvert [FONT_NAME.ttf] [FONT_SIZE] > [FONT_NAME.H]"
// Trying to use generated header file, I had this message: "Compilation error: stray '\377' in program". There was a problem with coding.
// I used this command in Git Bash to solve the problem: "iconv -f UTF-16 -t UTF-8 [OLD_NAME.h] > [NEW_NAME.h]"

// Recomendation: use following equation to define maximum font size: "FONT_SIZE = floor(HORISONTAL_DISPLAY_SIZE / SYMBOLS_NUMBER) - 1"  to display SNAKE title
// Recomendation: use following equation to define maximum font size: "FONT_SIZE = 6 * SNAKE_SPRITE_PIXEL_SIZE" to display SCORE


#include <random>
#include "settings.h" // Map, time and snake parameters
#include "text.h" // Start title text
#include "debug.h" // Functions for debugging
#include "display.h" // Display pins, constants and colors definition
#include "VCR_OSD_MONO_5.h" // VCR OSD MONO font, size 5
#include "VCR_OSD_MONO_21.h" // VCR OSD MONO font, size 21
#include "sprite.h" // Snake sprites bitmap
#include "button_matrix.h" // Keypad pins definition and buttons assignment


// Initialize random number generator
std::random_device RD; // Random device
std::mt19937 gen(RD());
std::uniform_int_distribution<> RND(0, MS - 1 - SSL); // Random number distribution
int FP = HSP; // Food position
bool FE; // Food has been eaten bool for next step


/// @brief Shift register for elements of the snake
/// @param arr Any array
/// @param size Array size
/// @param new_value New value of the array
template <typename T>
void shift_register(T arr[], int size, T new_value)
{
    for (int i = 0; i < size - 1; ++i)
    {
        arr[i] = arr[i + 1];
    }
    arr[size - 1] = new_value;
}


/// @brief Generate random number. If array element = 1, add 1 (dis is sht, u must redo dis)
int random_number()
{
    FP = RND(gen);
    while (MP[FP]) FP = (FP + 1) % MS; // TODO: REDO THIS SHT
    tft.drawBitmap(BX + SSS * (FP % MX) + 1, BY + SSS * (FP / MX) + 1, bitmap_food, SSS, SSS, colorO);
    return FP;
}


/// @brief Set start positions of head and tail. Place snake to the start position
void start()
{
    tft.fillScreen(colorBG);
	tft.drawRect(BX, BY, BW, BH, colorO);

    RST = false;
    FE = false;
    HP = HSP;
    TP = TSP;
    HCD = RIGHT;
    TCD = HCD;
    DA[MS] = NONE;
    SL = SSL;
    TI = STI;

    // Clear map and directions arrays
    for (bool &elem : MP) { elem = false; }
    for (Direction &elem : DA) { elem = NONE; }

    // Set map bools (TODO: other start directions support)
    MP[HSP] = true;
    tft.drawBitmap(BX + SSS * (HSP % MX) + 1, BY + SSS * (HSP / (MX - 1)) + 1, bitmap_head_r, SSS, SSS, colorO); // Draw head
    for (int i = 1; i < (SSL - 1); i++)
    { 
        MP[TSP + i] = true;
        tft.drawBitmap(BX + SSS * ((TSP % MX) + i) + 1, BY + SSS * (TSP / MX) + 1, bitmap_body_h, SSS, SSS, colorO); // Draw body
    }
    MP[TSP] = true;
    tft.drawBitmap(BX + SSS * (TSP % MX) + 1, BY + SSS * (TSP / MX) + 1, bitmap_tail_r, SSS, SSS, colorO); // Draw tail
    for (int i = 0; i < (SSL - 1); i++) { DA[std::size(DA) - 1 - i] = TSD; }


    // Print start score
    tft.setFont(&VCR_OSD_MONO21pt7b);
    tft.setCursor(CSPX_21, CSPY_21);
    tft.print("00");


    random_number();
}


/// @brief Snake makes one step: erasing the tail, rotating the head, collision check, sleeping
void step()
{

    if (HCD == -DA[std::size(DA) - 1]) HCD = DA[std::size(DA) - 1];

    tft.fillRect(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, SSS, SSS, colorBG); // Erase body part before head


    if (!FE)
    {
        switch (HCD)
        { 
            case RIGHT:
            {
                switch (DA[std::size(DA) - 1])
                {
                    case UP: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_dr, SSS, SSS, colorO); break; }
                    case DOWN: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_ur, SSS, SSS, colorO); break; }
                    default: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_body_h, SSS, SSS, colorO); }
                }
                break;
            }
            case LEFT:
            {
                switch (DA[std::size(DA) - 1])
                {
                    case UP: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_dl, SSS, SSS, colorO); break; }
                    case DOWN: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_ul, SSS, SSS, colorO); break; }
                    default: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_body_h, SSS, SSS, colorO); }
                }
                break;
            }
            case UP:
            {
                switch (DA[std::size(DA) - 1])
                {
                    case LEFT: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_ur, SSS, SSS, colorO); break; }
                    case RIGHT: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_ul, SSS, SSS, colorO); break; }
                    default: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_body_v, SSS, SSS, colorO); }
                }
                break;
            }
            case DOWN:
            {
                switch (DA[std::size(DA) - 1])
                {
                    case LEFT: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_dr, SSS, SSS, colorO); break; }
                    case RIGHT: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_turn_dl, SSS, SSS, colorO); break; }
                    default: { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_body_v, SSS, SSS, colorO); }
                }
                break;
            }
        }


        TCD = DA[std::size(DA) - SL + 1];

        tft.fillRect(BX + SSS * (TP % MX) + 1, BY + SSS * (TP / MX) + 1, SSS, SSS, colorBG); // Erase body part after tail

        MP[TP] = false;

        if ((TCD == LEFT) || (TCD == RIGHT))
        {
            TP = (TP / MX) * MX + ((TP + TCD) % MX);
            if (TP < 0) TP = MX - 1;
        }
        else 
        {
            TP = (TP + TCD) % MS;
            if (TP < 0) TP = MS + TP;
        }

        tft.fillRect(BX + SSS * (TP % MX) + 1, BY + SSS * (TP / MX) + 1, SSS, SSS, colorBG); // Erase tail

        switch (DA[std::size(DA) - SL + 2])
        {
            case LEFT: { tft.drawBitmap(BX + SSS * (TP % MX) + 1, BY + SSS * (TP / MX) + 1, bitmap_tail_l, SSS, SSS, colorO); break; }
            case RIGHT: { tft.drawBitmap(BX + SSS * (TP % MX) + 1, BY + SSS * (TP / MX) + 1, bitmap_tail_r, SSS, SSS, colorO); break; }
            case UP: { tft.drawBitmap(BX + SSS * (TP % MX) + 1, BY + SSS * (TP / MX) + 1, bitmap_tail_u, SSS, SSS, colorO); break; }
            case DOWN: { tft.drawBitmap(BX + SSS * (TP % MX) + 1, BY + SSS * (TP / MX) + 1, bitmap_tail_d, SSS, SSS, colorO); break; }
        }
    }
    else
    {
        FE = false;
        tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_full, SSS, SSS, colorO);
    }


    if ((HCD == LEFT) || (HCD == RIGHT))
    {
        HP = (HP / MX) * MX + ((HP + HCD) % MX);
        if (HP < 0) HP = MX - 1;
    }
    else
    {
        HP = (HP + HCD) % MS;
        if (HP < 0) HP = MS + HP;
    }


    tft.fillRect(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, SSS, SSS, colorBG); // Erase tile before placing head


    // Draw rotated head
    switch (HCD)
    {
        case LEFT:
        { 
            if ((((FP - HP) / MX) == 0) && ((FP - HP) < 0) && ((FP - HP) > (3 * LEFT))) { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_eat_l, SSS, SSS, colorO); }
            else { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_head_l, SSS, SSS, colorO); }
            break;
        }
        case RIGHT:
        { 
            if ((((FP - HP) / MX) == 0) && ((FP - HP) > 0) && ((FP - HP) < (3 * RIGHT))) { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_eat_r, SSS, SSS, colorO); }
            else { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_head_r, SSS, SSS, colorO); }
            break;
        }
        case UP:
        { 
            if ((((FP - HP) % MX) == 0) && ((FP - HP) < 0) && ((FP - HP) > (3 * UP))) { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_eat_u, SSS, SSS, colorO); }
            else { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_head_u, SSS, SSS, colorO); }
            break;
        }
        case DOWN:
        { 
            if ((((FP - HP) % MX) == 0) && ((FP - HP) > 0) && ((FP - HP) < (3 * DOWN))) { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_eat_d, SSS, SSS, colorO); }
            else { tft.drawBitmap(BX + SSS * (HP % MX) + 1, BY + SSS * (HP / MX) + 1, bitmap_head_d, SSS, SSS, colorO); }
            break;
        }
    }

    
    // Collision check
    if (MP[HP])
    {
        RST = true;
        delay(2000);
    }
    else
    {
        // Eating check
        if (FP == HP)
        {
            SL++;

            // Print score
            tft.setCursor(CSPX_21, CSPY_21);
            tft.fillRect(CSPX_21, BY, 2 * SX_21 + CSPX_21, 2 * (CSY_21 + 4), colorBG);
            if ((SL - SSL) < 10)
            {
                tft.printf("0%d", SL - SSL);
            }
            else if ((SL - SSL) < 100)
            {
                tft.printf("%d", SL - SSL);
            }
            else 
            {
                tft.print((SL - SSL) / 10);
                tft.setCursor(CSPX_21 + SX_21 / 2, CSPY_21 + CSY_21 + 4); // Magic number 4 is used for Y shift
                tft.print((SL - SSL) % 10);
            }

            TI--; // Speed up snake
            FE = true;
            int PFP = FP; // Previous food position
            while (FP == PFP) FP = random_number();
            Serial.println(FP);
            RND.param(std::uniform_int_distribution<>::param_type(0, MS - 1 - SL));
        }

        MP[HP] = true; // Move head to the new position

        shift_register(DA, std::size(DA), HCD); // Directions array shift
    }
}


void setup()
{
    Serial.begin(115200); // Baud rate

    // Initialize display
    tft.init(DX, DY);
    tft.setRotation(1);
    tft.invertDisplay(false);
    tft.fillScreen(colorBG);
    tft.setFont(&VCR_OSD_MONO5pt7b);
    tft.setTextColor(colorO);

    // SNAKE title print
    for (int i = 0; i < SNAKE_Y; i++)
    {
        tft.setCursor(CSPX_5, CSPY_5 + i * CSY_5);
        tft.print(SNAKE[i]); // Print on display
    }

    // Loading animation
    tft.setCursor(CSPX_5, CSPY_5 + ((SNAKE_Y + 3) * CSY_5));
    tft.println(BOOTING_UP);
    tft.drawRect(CSPX_5, CSPY_5 + ((SNAKE_Y + 4) * CSY_5), SX_5 * (SNAKE_X - 1) - 1, 2 * CSY_5, colorO); // Loading bar boundaries
    tft.fillRect(CSPX_5 + 1, CSPY_5 + ((SNAKE_Y + 4) * CSY_5) + 1, 0.05 * (SX_5 * (SNAKE_X - 1) - 3), 2 * CSY_5 - 2, colorL); // Loading 5%
    delay(150);
    tft.fillRect(CSPX_5 + 1, CSPY_5 + ((SNAKE_Y + 4) * CSY_5) + 1, 0.20 * (SX_5 * (SNAKE_X - 1) - 3), 2 * CSY_5 - 2, colorL); // Loading 20%
    delay(150);
    tft.fillRect(CSPX_5 + 1, CSPY_5 + ((SNAKE_Y + 4) * CSY_5) + 1, 0.50 * (SX_5 * (SNAKE_X - 1) - 3), 2 * CSY_5 - 2, colorL); // Loading 50%
    delay(150);
    tft.fillRect(CSPX_5 + 1, CSPY_5 + ((SNAKE_Y + 4) * CSY_5) + 1, 0.60 * (SX_5 * (SNAKE_X - 1) - 3), 2 * CSY_5 - 2, colorL); // Loading 60%
    delay(150);
    tft.fillRect(CSPX_5 + 1, CSPY_5 + ((SNAKE_Y + 4) * CSY_5) + 1, 0.80 * (SX_5 * (SNAKE_X - 1) - 3), 2 * CSY_5 - 2, colorL); // Loading 80%
    delay(150);
    tft.fillRect(CSPX_5 + 1, CSPY_5 + ((SNAKE_Y + 4) * CSY_5) + 1, (SX_5 * (SNAKE_X - 1) - 3), 2 * CSY_5 - 2, colorL); // Loaded!
    delay(150);

    start();
}



void loop()
{
    CM = millis();
    if (CM - PM >= TI)
    {
        PM = CM;
        if (RST) start();
        step();
    }
    
    keypad_read();
}
