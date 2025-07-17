#include "stdio.h"

#define DOGS_LENGTH 2

struct animal {
    char* name;
    long age;
};

struct dog {
    animal base;
    int toy;
};

int oldest_age;

void free_animal(animal* animal)
{
    free(animal->name);
}

void free_dog(dog* dog)
{
    free_animal(&dog->base);
}

char* str_clone(const char* str)
{
    char* result = (char*)malloc(strlen(str) + 1);

    memcpy(result, str, strlen(str)+1);

    return result;
}

int main(int argc, char** argv)
{
    dog* dogs[DOGS_LENGTH];

    dog* rex = (dog*)malloc(sizeof(dog));

    rex->base.name = str_clone("best rex");
    rex->base.age = 12;
    rex->toy = 0x30;

    dog toy = {
        .base.age = 7,
        .base.name = str_clone("best dog"),
        .toy = 0x12
    };

    dogs[0] = rex;
    dogs[1] = &toy;

    if (toy.base.age < rex->base.age)
    {
        printf("Toy is younger %d", toy.base.age);
    }
    else 
    {
        printf("Toy is older %d", toy.base.age);
    }
    
    oldest_age = 0;
    for (int i = 0; i < DOGS_LENGTH; i++) 
    {
        if (oldest_age < dogs[i]->base.age)
        {
            oldest_age = dogs[i]->base.age;
        }

        free_dog(dogs[i]);
    }
    free(rex);
}