#include "ButtonInput.h"

const uint8_t ButtonInput::buttonPin[] = {14, 12, 13};

ButtonInput::ButtonInput() {
  for (byte i = 0; i < 3; i++) {
    btns[i] = std::unique_ptr<Button>(new Button(buttonPin[i], 25, true, true));
  }
}

void ButtonInput::init() {
  for (byte i = 0; i < 3; i++) {
    btns[i]->begin();
    isButtonPushed[i] = false;
    isButtonLongPushed[i] = false;
    prevPushedButtonLong[i] = false;
    prevPagePushedButton[i] = btns[i]->isPressed();
  }
}

void ButtonInput::setEnableLongPush(bool left, bool center, bool right) {
  isEnableLongPush[0] = left;
  isEnableLongPush[1] = center;
  isEnableLongPush[2] = right;
}

void ButtonInput::update() {
  for (byte i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
    btns[i]->read();

    if (isEnableLongPush[i] && btns[i]->pressedFor(1000)) {
      if (prevPagePushedButton[i]) continue;
      isButtonLongPushed[i] = true;
      prevPushedButtonLong[i] = true;
    } else if (btns[i]->wasReleased()) {
      if (prevPushedButtonLong[i] || prevPagePushedButton[i]) {
        prevPushedButtonLong[i] = false;
        prevPagePushedButton[i] = false;
      } else {
        isButtonPushed[i] = true;
      }
    }
    // Serial.printf(
    //     "id:%d,prev:%d,isLong:%d,isButtonPushed:%d,wasReleased:%d,pressedFor:%"
    //     "d \n",
    //     i, prevPagePushedButton[i], prevPushedButtonLong[i],
    //     isButtonPushed[i], btns[i]->wasReleased(),
    //     btns[i]->pressedFor(1000));
  }
}

bool ButtonInput::isButtonPushedInternal(byte num) {
  bool temp = isButtonPushed[num];
  isButtonPushed[num] = false;
  return temp;
}
bool ButtonInput::isButtonPushedLongInternal(byte num) {
  bool temp = isButtonLongPushed[num];
  isButtonLongPushed[num] = false;
  return temp;
}

bool ButtonInput::isLeftPushed() { return isButtonPushedInternal(0); }
bool ButtonInput::isLeftPushedLong() { return isButtonPushedLongInternal(0); }
bool ButtonInput::isCenterPushed() { return isButtonPushedInternal(1); }
bool ButtonInput::isRightPushed() { return isButtonPushedInternal(2); }