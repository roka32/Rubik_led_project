#ifndef BUFFER_H
#define BUFFER_H
#include <Arduino.h>


class Buffer
{
    public:
        Buffer();
        void push(uint8_t );
        uint8_t popFirst();
        uint8_t getElement(uint8_t);
        bool isEmpty();
        uint8_t getNumOfItems();

    protected:

    private:
        static const uint8_t BUFF_SIZE = 50;
        uint8_t buff[BUFF_SIZE];
        uint8_t index = 0;
};

#endif // BUFFER_H
