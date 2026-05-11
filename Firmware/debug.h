/// @brief Print bool map array as 2D array
void print_map()
{
    for (int i = 0; i < MY; i++)
    {
        for (int j = 0; j < MX; j++)
        {
            Serial.print(MP[j + MX * i]);
            Serial.print(" ");
        }
        Serial.print("\n");
    }
    Serial.print("\n");
}