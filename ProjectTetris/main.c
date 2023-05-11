#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CANVAS_WIDTH 10
#define CANVAS_HEIGHT 20

typedef enum
{
	RED = 41,
	GREEN,
	YELLOW,
	BLUE,
	PURPLE,
	CYAN,
	WHITE,
	BLACK = 0,
}Color;

typedef enum
{
	EMPTY = -1,
	I,
	J,
	L,
	O,
	S,
	T,
	Z
}ShapeId;

typedef struct
{
	ShapeId shape;
	Color color;
	int size;
	char rotates[4][4][4]; //4­Ó¤è¦V
}Shape;

typedef struct
{
	int x;
	int y;
	int score;
	int rotates;
	int fallTime;
	ShapeId queue[4];
}State;

Shape shapes[7] = {
    {
        .shape = I,
        .color = CYAN,
        .size = 4,
        .rotates =
        {
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}
            },
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}
            }
        }
    },
    {
        .shape = J,
        .color = BLUE,
        .size = 3,
        .rotates =
        {
            {
                {1, 0, 0},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 1},
                {0, 1, 0},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {1, 1, 0}
            }
        }
    },
    {
        .shape = L,
        .color = YELLOW,
        .size = 3,
        .rotates =
        {
            {
                {0, 0, 1},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {1, 0, 0}
            },
            {
                {1, 1, 0},
                {0, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = O,
        .color = WHITE,
        .size = 2,
        .rotates =
        {
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            }
        }
    },
    {
        .shape = S,
        .color = GREEN,
        .size = 3,
        .rotates =
        {
            {
                {0, 1, 1},
                {1, 1, 0},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 0, 0},
                {0, 1, 1},
                {1, 1, 0}
            },
            {
                {1, 0, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = T,
        .color = PURPLE,
        .size = 3,
        .rotates =
        {
            {
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0}
            },

                {{0, 1, 0},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = Z,
        .color = RED,
        .size = 3,
        .rotates =
        {
            {
                {1, 1, 0},
                {0, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 0, 1},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {1, 0, 0}
            }
        }
    },
};

typedef struct
{
	Color color;
	ShapeId shape;
	bool current;
}Block;

void resetBlock(Block* block)
{
	block->color = BLACK;
	block->shape = EMPTY;
	block->current = false;
}

int main()
{
	srand(time(NULL));
	State state =
	{
		.x = CANVAS_WIDTH / 2,
		.y = 0,
		.score = 0,
		.rotates = 0,
		.fallTime = 0
	};

	for (int i = 0; i < 4; i++)
	{
		state.queue[i] = rand() % 7;
	}

	Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
	for (int i = 0; i < CANVAS_HEIGHT; i++)
	{
		for (int j = 0; j < CANVAS_WIDTH; j++)
		{
			resetBlock(&canvas[i][j]);
		}
	}

	system("cls");
	printf("\e[?251"); // hide cursor

	/*Color cur;
	for (int i = 0; i < 7; i++)
	{
		for (int s = 0; s < shape[i].size; s++)
		{
			for (int t = 0; t < shape[i].size; t++)
			{
				for (int r = 0; r < 4; r++)
				{
					if (shape[i].rotates[r][s][t])
					{
						cur = shape[i].color;
					}
					else
					{
						cur = BLACK;
					}
					printf("\033[%dm \033[0m", cur);
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
	}
	return 0;*/
}