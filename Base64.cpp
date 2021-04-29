/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 14:38:10 by dnakano           #+#    #+#             */
/*   Updated: 2021/04/29 16:52:57 by dnakano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base64.hpp"
#include <iostream>

std::map<char, char> Base64::encodemap;

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
