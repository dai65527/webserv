/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 14:38:10 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/29 17:37:27 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base64.hpp"

std::map<char, char> Base64::encodemap;
std::map<char, char> Base64::decodemap;

std::string Base64::encode(const std::string& data) {
  return encode(reinterpret_cast<const unsigned char*>(&data[0]), data.size());
}

std::string Base64::encode(const unsigned char* data, size_t length) {
  size_t i_org = 0;
  size_t i_res = 0;
  std::string res;
  size_t res_len = ((length * 4) % 3) ? length * 4 / 3 + 1 : length * 4 / 3;
  res.resize((res_len % 4) ? (res_len + 4 - (res_len % 4)) : res_len);

  while (i_org < length) {
    res[i_res] = data[i_org] >> 2;
    res[i_res + 1] = (data[i_org] << 4) & 0b00111111;

    if (i_org + 1 < length) {
      res[i_res + 1] |= data[i_org + 1] >> 4;
      res[i_res + 2] = data[i_org + 1] << 2 & 0b00111111;
    } else {
      res[i_res + 2] = '=';
      res[i_res + 3] = '=';
      break;
    }

    if (i_org + 2 < length) {
      res[i_res + 2] |= data[i_org + 2] >> 6;
      res[i_res + 3] = data[i_org + 2] & 0b00111111;
    } else {
      res[i_res + 3] = '=';
    }
    i_org += 3;
    i_res += 4;
  }

  initEncodemap();
  for (size_t i = 0; i < res_len; ++i) {
    res[i] = encodemap[res[i]];
  }
  return res;
}

std::string Base64::decode(const std::string& data) {
  std::string res;
  size_t len = data.length();

  initDecodemap();

  char c;
  size_t i_org = 0;
  while (i_org < len && data[i_org] != '=') {
    c = decodemap[data[i_org]] << 2;
    if (i_org + 1 >= len || data[i_org + 1] == '=') {
      if (c) res.append(&c, 1);
      break;
    }
    c |= (decodemap[data[i_org + 1]] & 0b00111111) >> 4;
    if (c) res.append(&c, 1);

    c = decodemap[data[i_org + 1]] << 4;
    if (i_org + 2 >= len || data[i_org + 2] == '=') {
      if (c) res.append(&c, 1);
      break;
    }
    c |= (decodemap[data[i_org + 2]] & 0b00111111) >> 2;
    if (c) res.append(&c, 1);

    c = decodemap[data[i_org + 2]] << 6;
    if (i_org + 3 >= len || data[i_org + 3] == '=') {
      if (c) res.append(&c, 1);
      break;
    }
    c |= (decodemap[data[i_org + 3]] & 0b00111111);
    if (c) res.append(&c, 1);

    i_org += 4;
  }

  return res;
}

void Base64::initEncodemap() {
  if (!encodemap.empty()) {
    return;
  }

  encodemap[0] = 'A';
  encodemap[1] = 'B';
  encodemap[2] = 'C';
  encodemap[3] = 'D';
  encodemap[4] = 'E';
  encodemap[5] = 'F';
  encodemap[6] = 'G';
  encodemap[7] = 'H';
  encodemap[8] = 'I';
  encodemap[9] = 'J';
  encodemap[10] = 'K';
  encodemap[11] = 'L';
  encodemap[12] = 'M';
  encodemap[13] = 'N';
  encodemap[14] = 'O';
  encodemap[15] = 'P';
  encodemap[16] = 'Q';
  encodemap[17] = 'R';
  encodemap[18] = 'S';
  encodemap[19] = 'T';
  encodemap[20] = 'U';
  encodemap[21] = 'V';
  encodemap[22] = 'W';
  encodemap[23] = 'X';
  encodemap[24] = 'Y';
  encodemap[25] = 'Z';
  encodemap[26] = 'a';
  encodemap[27] = 'b';
  encodemap[28] = 'c';
  encodemap[29] = 'd';
  encodemap[30] = 'e';
  encodemap[31] = 'f';
  encodemap[32] = 'g';
  encodemap[33] = 'h';
  encodemap[34] = 'i';
  encodemap[35] = 'j';
  encodemap[36] = 'k';
  encodemap[37] = 'l';
  encodemap[38] = 'm';
  encodemap[39] = 'n';
  encodemap[40] = 'o';
  encodemap[41] = 'p';
  encodemap[42] = 'q';
  encodemap[43] = 'r';
  encodemap[44] = 's';
  encodemap[45] = 't';
  encodemap[46] = 'u';
  encodemap[47] = 'v';
  encodemap[48] = 'w';
  encodemap[49] = 'x';
  encodemap[50] = 'y';
  encodemap[51] = 'z';
  encodemap[52] = '0';
  encodemap[53] = '1';
  encodemap[54] = '2';
  encodemap[55] = '3';
  encodemap[56] = '4';
  encodemap[57] = '5';
  encodemap[58] = '6';
  encodemap[59] = '7';
  encodemap[60] = '8';
  encodemap[61] = '9';
  encodemap[62] = '+';
  encodemap[63] = '/';
}

void Base64::initDecodemap() {
  if (!decodemap.empty()) {
    return;
  }
  decodemap['A'] = 0;
  decodemap['B'] = 1;
  decodemap['C'] = 2;
  decodemap['D'] = 3;
  decodemap['E'] = 4;
  decodemap['F'] = 5;
  decodemap['G'] = 6;
  decodemap['H'] = 7;
  decodemap['I'] = 8;
  decodemap['J'] = 9;
  decodemap['K'] = 10;
  decodemap['L'] = 11;
  decodemap['M'] = 12;
  decodemap['N'] = 13;
  decodemap['O'] = 14;
  decodemap['P'] = 15;
  decodemap['Q'] = 16;
  decodemap['R'] = 17;
  decodemap['S'] = 18;
  decodemap['T'] = 19;
  decodemap['U'] = 20;
  decodemap['V'] = 21;
  decodemap['W'] = 22;
  decodemap['X'] = 23;
  decodemap['Y'] = 24;
  decodemap['Z'] = 25;
  decodemap['a'] = 26;
  decodemap['b'] = 27;
  decodemap['c'] = 28;
  decodemap['d'] = 29;
  decodemap['e'] = 30;
  decodemap['f'] = 31;
  decodemap['g'] = 32;
  decodemap['h'] = 33;
  decodemap['i'] = 34;
  decodemap['j'] = 35;
  decodemap['k'] = 36;
  decodemap['l'] = 37;
  decodemap['m'] = 38;
  decodemap['n'] = 39;
  decodemap['o'] = 40;
  decodemap['p'] = 41;
  decodemap['q'] = 42;
  decodemap['r'] = 43;
  decodemap['s'] = 44;
  decodemap['t'] = 45;
  decodemap['u'] = 46;
  decodemap['v'] = 47;
  decodemap['w'] = 48;
  decodemap['x'] = 49;
  decodemap['y'] = 50;
  decodemap['z'] = 51;
  decodemap['0'] = 52;
  decodemap['1'] = 53;
  decodemap['2'] = 54;
  decodemap['3'] = 55;
  decodemap['4'] = 56;
  decodemap['5'] = 57;
  decodemap['6'] = 58;
  decodemap['7'] = 59;
  decodemap['8'] = 60;
  decodemap['9'] = 61;
  decodemap['+'] = 62;
  decodemap['/'] = 63;
}
