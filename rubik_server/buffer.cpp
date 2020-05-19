#include "buffer.h"

Buffer::Buffer()
{
    for(int i = 0; i<BUFF_SIZE; i++){
        buff[i]=0;
    }
}

void Buffer::push(uint8_t get){
   if(index<BUFF_SIZE){
    buff[index]=get;
    if(buff[0] != 0)index++;
    }else{
        Serial.println("ERROR: Buffer is full!!");
        
    }
}

uint8_t Buffer::popFirst(){
    uint8_t tmp = buff[0];
    for(int i = 0; i<index-1; i++){

        buff[i]=buff[i+1];


    }
    buff[index-1]=0;
    if(tmp != 0)index--;
    return tmp;
}

uint8_t Buffer::getElement(uint8_t num){
    return buff[num];
}

bool Buffer::isEmpty(){
    return buff[0] == 0 ? true : false;
}

uint8_t Buffer::getNumOfItems(){
    return index-1;
}
