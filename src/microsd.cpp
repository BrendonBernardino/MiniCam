#include "microsd.h"

void initMicroSDCard() {
  printf("Mounting MicroSD Card\n");
  if(!SD_MMC.begin()) {
    printf("MicroSD Card Mount Failed\n");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE) {
    printf("No MicroSD Card found\n");
    return;
  }
}