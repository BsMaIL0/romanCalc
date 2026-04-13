#include <fstream>
#include <iostream>

int romanToArabic(const char *roman, int len) {
  int result = 0;

  for (int i = 0; i < len; ++i) {
    switch (roman[i]) {
    case 'M':
      result += 1000;
      break;
    case 'D':
      result += 500;
      break;
    case 'C':
      if (i + 1 < len && (roman[i + 1] == 'M' || roman[i + 1] == 'D')) {
        result -= 100;
      } else {
        result += 100;
      }
      break;
    case 'L':
      result += 50;
      break;
    case 'X':
      if (i + 1 < len && (roman[i + 1] == 'C' || roman[i + 1] == 'L')) {
        result -= 10;
      } else {
        result += 10;
      }
      break;
    case 'V':
      result += 5;
      break;
    case 'I':
      if (i + 1 < len && (roman[i + 1] == 'V' || roman[i + 1] == 'X')) {
        result -= 1;
      } else {
        result += 1;
      }
      break;
    default:
      return -1;
    }
  }
  return result;
}

void arabicToRoman(int number, char *result) {
  static int arabNumber[] = {1000, 900, 500, 400, 100, 90, 50,
                             40,   10,  9,   5,   4,   1};
  static const char *romanNumbers[] = {"M",  "CM", "D",  "CD", "C",  "XC", "L",
                                       "XL", "X",  "IX", "V",  "IV", "I"};

  int index = 0;

  for (int i = 0; i < 13 && number > 0; ++i) {
    while (number >= arabNumber[i]) {
      for (int k = 0; romanNumbers[i][k] != 0; ++k) {
        result[index++] = romanNumbers[i][k];
      }
      number -= arabNumber[i];
    }
  }
  result[index] = '\0';
}

bool isOperator(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Wrong arguments!" << std::endl;
    return 1;
  }

  std::ifstream in(argv[1]);
  std::ofstream out(argv[2]);

  if (!in) {
    std::cerr << "Cannot open input file" << std::endl;
    return 1;
  }
  if (!out) {
    std::cerr << "Cannot open output file" << std::endl;
    return 1;
  }

  char line[1000];
  char result[100];

  while (in.getline(line, 1000)) {
    out << line;

    int stack[100];
    int top = -1;
    bool error = false;

    int i = 0;

    while (line[i] != '\0' && !error) {
      while (line[i] == ' ') {
        i++;
      }
      if (line[i] == '\0') {
        break;
      }

      if (isOperator(line[i]) && (line[i + 1] == ' ' || line[i + 1] == '\0')) {

        if (top < 1) {
          error = true;
          break;
        }

        int b = stack[top--];
        int a = stack[top--];

        int calc_result;
        switch (line[i]) {
        case '+':
          calc_result = a + b;
          break;
        case '-':
          calc_result = a - b;
          break;
        case '*':
          calc_result = a * b;
          break;
        case '/':
          if (b == 0) {
            error = true;
            break;
          }
          calc_result = a / b;
          break;
        default:
          error = true;
          break;
        }

        if (error)
          break;

        stack[++top] = calc_result;
        i++;
      } else {
        char number[10] = {0};
        int idx = 0;

        while (line[i] != ' ' && line[i] != '\0') {
          number[idx++] = line[i++];
        }

        int value = romanToArabic(number, idx);

        if (value < 1 || value > 3999) {
          error = true;
          break;
        }

        if (top + 1 >= 100) { 
          error = true;
          break;
        }
        stack[++top] = value;
      }
    }

    if (!error && top == 0) {
      int final_result = stack[0];
      if (final_result >= 1 && final_result <= 3999) {
        arabicToRoman(final_result, result);
        out << " = " << result << std::endl;
      } else {
        out << " = ERR" << std::endl;
      }
    } else {
      out << " = ERR" << std::endl;
    }
  }

  in.close();
  out.close();
  return 0;
}
