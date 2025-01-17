/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the terms found in the LICENSE file in the root of this source tree.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include <stdint.h>

#include "lte/gateway/c/core/oai/common/TLVEncoder.h"
#include "lte/gateway/c/core/oai/common/TLVDecoder.h"
#include "lte/gateway/c/core/oai/tasks/nas/emm/msg/CsServiceNotification.h"
#include "lte/gateway/c/core/oai/common/common_defs.h"

int decode_cs_service_notification(
    cs_service_notification_msg* cs_service_notification, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, CS_SERVICE_NOTIFICATION_MINIMUM_LENGTH, len);

  /*
   * Decoding mandatory fields
   */
  if ((decoded_result =
           decode_paging_identity(&cs_service_notification->pagingidentity, 0,
                                  buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  /*
   * Decoding optional fields
   */
  while (len > decoded) {
    uint8_t ieiDecoded = *(buffer + decoded);

    /*
     * Type | value iei are below 0x80 so just return the first 4 bits
     */
    if (ieiDecoded >= 0x80) ieiDecoded = ieiDecoded & 0xf0;

    switch (ieiDecoded) {
      case CS_SERVICE_NOTIFICATION_CLI_IEI:
        if ((decoded_result = decode_cli(&cs_service_notification->cli,
                                         CS_SERVICE_NOTIFICATION_CLI_IEI,
                                         buffer + decoded, len - decoded)) <= 0)
          return decoded_result;

        decoded += decoded_result;
        /*
         * Set corresponding mask to 1 in presencemask
         */
        cs_service_notification->presencemask |=
            CS_SERVICE_NOTIFICATION_CLI_PRESENT;
        break;

      case CS_SERVICE_NOTIFICATION_SS_CODE_IEI:
        if ((decoded_result =
                 decode_ss_code(&cs_service_notification->sscode,
                                CS_SERVICE_NOTIFICATION_SS_CODE_IEI,
                                buffer + decoded, len - decoded)) <= 0)
          return decoded_result;

        decoded += decoded_result;
        /*
         * Set corresponding mask to 1 in presencemask
         */
        cs_service_notification->presencemask |=
            CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT;
        break;

      case CS_SERVICE_NOTIFICATION_LCS_INDICATOR_IEI:
        if ((decoded_result =
                 decode_lcs_indicator(&cs_service_notification->lcsindicator,
                                      CS_SERVICE_NOTIFICATION_LCS_INDICATOR_IEI,
                                      buffer + decoded, len - decoded)) <= 0)
          return decoded_result;

        decoded += decoded_result;
        /*
         * Set corresponding mask to 1 in presencemask
         */
        cs_service_notification->presencemask |=
            CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT;
        break;

      case CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_IEI:
        if ((decoded_result = decode_lcs_client_identity(
                 &cs_service_notification->lcsclientidentity,
                 CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_IEI,
                 buffer + decoded, len - decoded)) <= 0)
          return decoded_result;

        decoded += decoded_result;
        /*
         * Set corresponding mask to 1 in presencemask
         */
        cs_service_notification->presencemask |=
            CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT;
        break;

      default:
        errorCodeDecoder = TLV_UNEXPECTED_IEI;
        return TLV_UNEXPECTED_IEI;
    }
  }

  return decoded;
}

int encode_cs_service_notification(
    cs_service_notification_msg* cs_service_notification, uint8_t* buffer,
    uint32_t len) {
  int encoded = 0;
  int encode_result = 0;

  /*
   * Checking IEI and pointer
   */
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, CS_SERVICE_NOTIFICATION_MINIMUM_LENGTH, len);

  if ((encode_result = encode_paging_identity(
           &cs_service_notification->pagingidentity, 0, buffer + encoded,
           len - encoded)) < 0)  // Return in case of error
    return encode_result;
  else
    encoded += encode_result;

  if ((cs_service_notification->presencemask &
       CS_SERVICE_NOTIFICATION_CLI_PRESENT) ==
      CS_SERVICE_NOTIFICATION_CLI_PRESENT) {
    if ((encode_result = encode_cli(cs_service_notification->cli,
                                    CS_SERVICE_NOTIFICATION_CLI_IEI,
                                    buffer + encoded, len - encoded)) < 0)
      // Return in case of error
      return encode_result;
    else
      encoded += encode_result;
  }

  if ((cs_service_notification->presencemask &
       CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT) ==
      CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT) {
    if ((encode_result = encode_ss_code(&cs_service_notification->sscode,
                                        CS_SERVICE_NOTIFICATION_SS_CODE_IEI,
                                        buffer + encoded, len - encoded)) < 0)
      // Return in case of error
      return encode_result;
    else
      encoded += encode_result;
  }

  if ((cs_service_notification->presencemask &
       CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT) ==
      CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT) {
    if ((encode_result =
             encode_lcs_indicator(&cs_service_notification->lcsindicator,
                                  CS_SERVICE_NOTIFICATION_LCS_INDICATOR_IEI,
                                  buffer + encoded, len - encoded)) < 0)
      // Return in case of error
      return encode_result;
    else
      encoded += encode_result;
  }

  if ((cs_service_notification->presencemask &
       CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT) ==
      CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT) {
    if ((encode_result = encode_lcs_client_identity(
             cs_service_notification->lcsclientidentity,
             CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_IEI, buffer + encoded,
             len - encoded)) < 0)
      // Return in case of error
      return encode_result;
    else
      encoded += encode_result;
  }

  return encoded;
}
