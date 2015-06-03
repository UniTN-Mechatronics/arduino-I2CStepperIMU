
bool process_serial() {
  char ch;
  static int v = 0;

  if (Serial.available()) {
    ch = Serial.read();
    // Serial command parsing:
    switch (ch) {
      case '?':
        Serial.println("---");
        Serial.print("p: ");
        Serial.println(g_state.pos.l);
        Serial.print("s: ");
        Serial.println(g_state.speed, DEC);
        Serial.print("v: ");
        Serial.println(v);
        Serial.print("c: ");
        Serial.println(g_state.speed_ctrl);
        Serial.print("w: ");
        Serial.println(g_state.pwr);
        PROMPT;
        break;
      case '0'...'9': // Accumulates values
        v = v * 10 + ch - '0';
        break;
        return false;
      default:
        v = command_parsing(ch, v);
    }
  }
  return false;
}

