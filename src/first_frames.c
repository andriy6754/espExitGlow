#include "../include/led_screen.h"

const rgb_t *rainStart[SIZE][SIZE] = 
{
	{&wRainy.null, &wRainy.null, &wRainy.n3, &wRainy.n3, &wRainy.n4, &wRainy.n4, &wRainy.n5, &wRainy.null},
	{&wRainy.null, &wRainy.n2, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n4, &wRainy.n4, &wRainy.n5},
	{&wRainy.n1, &wRainy.n2, &wRainy.n2, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n4, &wRainy.n4},
	{&wRainy.n1, &wRainy.n1, &wRainy.n2, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n4},
	{&wRainy.null, &wRainy.n1, &wRainy.n2, &wRainy.n3, &wRainy.null, &wRainy.null, &wRainy.n4, &wRainy.null},
	{&wRainy.null, &wRainy.null, &wRainy.n6, &wRainy.null, &wRainy.null, &wRainy.n6, &wRainy.null, &wRainy.null},
	{&wRainy.null, &wRainy.null, &wRainy.null, &wRainy.n6, &wRainy.null, &wRainy.null, &wRainy.n6, &wRainy.null},
	{&wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null},
};

/*const rgb_t *showerStart[SIZE][SIZE] = 
{
	{&wRainy.null, &wRainy.null, &wRainy.n3, &wRainy.n3, &wRainy.n4, &wRainy.n4, &wRainy.n5, &wRainy.null},
	{&wRainy.null, &wRainy.n2, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n4, &wRainy.n4, &wRainy.n5},
	{&wRainy.n1, &wRainy.n2, &wRainy.n2, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n4, &wRainy.n4},
	{&wRainy.n1, &wRainy.n1, &wRainy.n2, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n3, &wRainy.n4},
	{&wRainy.n6, &wRainy.n1, &wRainy.n2, &wRainy.n3, &wRainy.null, &wRainy.null, &wRainy.n4, &wRainy.null},
	{&wRainy.n6, &wRainy.null, &wRainy.null, &wRainy.n6, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null},
	{&wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null},
	{&wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null, &wRainy.null},
};*/

/*const rgb_t *drizzleStart[SIZE][SIZE] = 
{
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n5, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n5},
    {&wSnowyNDrizzly.n1, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n4},
    {&wSnowyNDrizzly.n1, &wSnowyNDrizzly.n1, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.n1, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n6, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null},
};*/

const rgb_t *cloudyStart[SIZE][SIZE] = 
{
    {&wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.null},
    {&wSunny.null, &wSunny.n5, &wSunny.n5, &wSunny.n1, &wSunny.n1, &wSunny.n2, &wSunny.n1, &wSunny.n1},
    {&wSunny.n6, &wSunny.n5, &wSunny.n5, &wSunny.n1, &wSunny.n2, &wSunny.n1, &wSunny.n1, &wSunny.n1},
    {&wSunny.n6, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n1},
    {&wSunny.n6, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n1, &wSunny.n5, &wSunny.n5},
    {&wSunny.n6, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n5},
    {&wSunny.null, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.n5, &wSunny.null, &wSunny.n5, &wSunny.n5},
    {&wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null},
};

const rgb_t *sunnyStart[SIZE][SIZE] = 
{
    {&wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null},
    {&wSunny.null, &wSunny.null, &wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.null, &wSunny.null, &wSunny.null},
    {&wSunny.null, &wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.n2, &wSunny.n1, &wSunny.null, &wSunny.null},
    {&wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n2, &wSunny.n1, &wSunny.null},
    {&wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.null},
    {&wSunny.null, &wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.n1, &wSunny.null, &wSunny.null},
    {&wSunny.null, &wSunny.null, &wSunny.null, &wSunny.n1, &wSunny.n1, &wSunny.null, &wSunny.null, &wSunny.null},
    {&wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null, &wSunny.null},
};

const rgb_t *snowyStart[SIZE][SIZE] = 
{
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n5, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n5},
    {&wSnowyNDrizzly.n1, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.n4},
    {&wSnowyNDrizzly.n1, &wSnowyNDrizzly.n1, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.n4},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.n1, &wSnowyNDrizzly.n2, &wSnowyNDrizzly.n3, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n4, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n7, &wSnowyNDrizzly.n8, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.n8, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null},
    {&wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null, &wSnowyNDrizzly.null},
};

