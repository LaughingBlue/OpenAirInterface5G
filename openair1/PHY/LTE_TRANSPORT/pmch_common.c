/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
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

#include "PHY/defs_eNB.h"
#include "PHY/phy_extern.h"

int is_pmch_subframe(uint32_t frame, int subframe, LTE_DL_FRAME_PARMS *frame_parms)
{

  uint32_t period;
  uint8_t i;
  uint8_t j;

  //  LOG_D(PHY,"is_pmch_subframe: frame %d, subframe %d, num_MBSFN_config %d\n",
  //  frame,subframe,frame_parms->num_MBSFN_config);

  for (i=0; i<frame_parms->num_MBSFN_config; i++) {  // we have at least one MBSFN configuration
    period = 1<<frame_parms->MBSFN_config[i].radioframeAllocationPeriod;

    if (frame_parms->MBSFN_config[i].fourFrames_flag == 0) {
      if ((frame % period) == frame_parms->MBSFN_config[i].radioframeAllocationOffset) {
        if (frame_parms->frame_type == FDD) {
          switch (subframe) {

          case 1:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_FDD_SF1) > 0)
              return(1);

            break;

          case 2:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_FDD_SF2) > 0)
              return(1);

            break;

          case 3:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_FDD_SF3) > 0)
              return(1);

            break;

          case 6:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_FDD_SF6) > 0)
              return(1);

            break;

          case 7:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_FDD_SF7) > 0)
              return(1);

            break;

          case 8:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_FDD_SF8) > 0)
              return(1);

            break;
          }
        } else  {
          switch (subframe) {
          case 3:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_TDD_SF3) > 0)
              return(1);

            break;

          case 4:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_TDD_SF4) > 0)
              return(1);

            break;

          case 7:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_TDD_SF7) > 0)
              return(1);

            break;

          case 8:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_TDD_SF8) > 0)
              return(1);

            break;

          case 9:
            if ((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig & MBSFN_TDD_SF9) > 0)
              return(1);

            break;
          }
        }
      }

    } else { // handle 4 frames case

      for(j=0;j<4;j++) {
        if ((frame % period) == (frame_parms->MBSFN_config[i].radioframeAllocationOffset + j)) {
          if (frame_parms->frame_type == FDD) {
            switch (subframe) {
            case 1:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig)  & (0x800000>>(j*6))) > 0) {
                //LOG_E(PHY,"SubframeConfig<<6(%x),MBSFN_FDD_SF1(%x)\n",frame_parms->MBSFN_config[i].mbsfn_SubframeConfig,0x800000>>(j*6));
                return(1);
              }

              break;

            case 2:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig) & (0x400000>>(j*6))) > 0) {
                //LOG_E(PHY,"SubframeConfig<<6(%x),MBSFN_FDD_SF1(%x)\n",frame_parms->MBSFN_config[i].mbsfn_SubframeConfig,0x400000>>(j*6));
                return(1);
              }

              break;

            case 3:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig) & (0x200000>>(j*6))) > 0) {
                //LOG_E(PHY,"SubframeConfig<<6(%x),MBSFN_FDD_SF1(%x)\n",frame_parms->MBSFN_config[i].mbsfn_SubframeConfig,0x200000>>(j*6));
                return(1);
              }

              break;

            case 6:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig) & (0x100000>>(j*6))) > 0) {
                //LOG_E(PHY,"SubframeConfig<<6(%x),MBSFN_FDD_SF1(%x)\n",frame_parms->MBSFN_config[i].mbsfn_SubframeConfig,0x100000>>(j*6));
                return(1);
              }

              break;

            case 7:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig) & (0x80000>>(j*6))) > 0) {
                //LOG_E(PHY,"SubframeConfig<<6(%x),MBSFN_FDD_SF1(%x)\n",frame_parms->MBSFN_config[i].mbsfn_SubframeConfig,0x80000>>(j*6));
                return(1);
              }

              break;

            case 8:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig) & (0x40000>>(j*6))) > 0) {
                //LOG_E(PHY,"SubframeConfig<<6(%x),MBSFN_FDD_SF1(%x)\n",frame_parms->MBSFN_config[i].mbsfn_SubframeConfig,0x40000>>(j*6));
                return(1);
              }

              break;
            }
          } else {
            switch (subframe) {
            case 3:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig>>j*6) & MBSFN_TDD_SF3) > 0)
                return(1);

              break;

            case 4:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig>>j*6) & MBSFN_TDD_SF4) > 0)
                return(1);

              break;

            case 7:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig>>j*6) & MBSFN_TDD_SF7) > 0)
                return(1);

              break;

            case 8:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig>>j*6) & MBSFN_TDD_SF8) > 0)
                return(1);

              break;

            case 9:
              if (((frame_parms->MBSFN_config[i].mbsfn_SubframeConfig>>j*6) & MBSFN_TDD_SF9) > 0)
                return(1);

              break;
            }
          }
        }
      }
    }
  }

  return(0);
}
