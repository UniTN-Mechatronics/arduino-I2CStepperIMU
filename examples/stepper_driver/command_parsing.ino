int command_parsing(char ch, int v) {
  switch (ch) {
    case 'p':
      Serial.print("p: ");
      Serial.println(g_state.pos.l);
      PROMPT;
      break;
    case 'R':
      g_state.speed_ctrl = true;
      g_state.spwr = (v == 0 ? PWM_PWR : constrain(v, 0, PWM_PWR));
      Serial.print("c: ");
      Serial.println(g_state.speed_ctrl ? "on" : "off");
      Serial.print("w: ");
      Serial.println(g_state.spwr);
      v = 0;
      PROMPT;
      break;
    case 'r':
      g_state.speed_ctrl = false;
      Serial.print("c: ");
      Serial.println(g_state.speed_ctrl ? "on" : "off");
      PROMPT;
      break;
    case 'S':
      g_state.speed = constrain(v, 0, MAX_SPEED);
      Serial.print("s: ");
      Serial.println(g_state.speed, DEC);
      v = 0;
      PROMPT;
      break;
    case 's':
      g_state.speed = constrain(-v, -MAX_SPEED, 0);
      Serial.print("s: ");
      Serial.println(g_state.speed, DEC);
      v = 0;
      PROMPT;
      break;
    case '+':
      if (g_state.speed != 0) {
        v = v == 0 ? 1 : v;
        myStepper.setSpeed(abs(g_state.speed));
        myStepper.step(v);
      }
      else {
        myStepper.setSpeed(MAX_SPEED);
        myStepper.step(v = 1);
        myStepper.setSpeed(abs(g_state.speed));
      }
      g_state.pos.l += v;
      Serial.println(g_state.pos.l);
      v = 0;
      PROMPT;
      break;
    case '-':
      if (g_state.speed != 0) {
        v = v == 0 ? 1 : v;
        myStepper.setSpeed(abs(g_state.speed));
        myStepper.step(-v);
      }
      else {
        myStepper.setSpeed(MAX_SPEED);
        myStepper.step(v = -1);
        myStepper.setSpeed(abs(g_state.speed));
      }
      g_state.pos.l -= v;
      Serial.println(g_state.pos.l);
      v = 0;
      PROMPT;
      break;
    case 'z':
      g_state.pos.l = 0;
      v = 0;
      PROMPT;
      break;
    case 'Z':
      if (g_state.speed != 0) {
        myStepper.setSpeed(abs(g_state.speed));
        myStepper.step(-g_state.pos.l);
        g_state.pos.l = 0;
        v = 0;
      }
      PROMPT;
      break;
    case 'W':
      g_state.pwr = v;
      Serial.print("w: ");
      Serial.println(g_state.pwr, DEC);
      v = 0;
      PROMPT;
      break;
    case 'w':
      g_state.pwr = 0;
      v = 0;
      PROMPT;
      break;
    case 'i':
      EEPROM.put(g_eeprom_addr, (byte)v);
      v = 0;
      PROMPT;
      break;
    default:
      v = 0;
      PROMPT;
  }
  return v;
}

