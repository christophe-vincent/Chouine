// ChouineTest.cpp : Defines the entry point for the console application.
//

#include "Chouine.h"

string getCardcolor(int _id)
{
   string ret = "";
   switch (_id >> 3)
   {
   case 0: ret = "Coeur"; break;
   case 1: ret = "Pic"; break;
   case 2: ret = "Trefle"; break;
   case 3: ret = "Carreau"; break;
   default: ret = "???";
   }
   return ret;
}


int main()
{
   Chouine *chouine = Chouine::getInstance();

   chouine->newGame();
   chouine->setPlayerLevel(0, 5);
   chouine->setPlayerLevel(0, 0);

   //chouine->


   return 0;
}

