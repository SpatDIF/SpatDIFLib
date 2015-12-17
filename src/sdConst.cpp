/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama, Trond Lossius
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#include "sdConst.h"

const std::map<EExtension, sdExtension::sdExtensionSpec>  sdExtension::extensionDict = {
    {EExtension::SD_CORE, sdExtensionSpec("core",
                                          {{SD_TYPE, "type"},
                                              {SD_PRESENT, "present"},
                                              {SD_POSITION, "position"},
                                              {SD_ORIENTATION, "orientation"}})},
    {EExtension::SD_MEDIA, sdExtensionSpec("media",
                                           {{SD_MEDIA_ID, "id"},
                                               {SD_MEDIA_TYPE, "type"},
                                               {SD_MEDIA_LOCATION, "location"},
                                               {SD_MEDIA_CHANNEL, "channel"},
                                               {SD_MEDIA_GAIN, "gain"},
                                               {SD_MEDIA_TIME_OFFSET, "time-offset"}})},
    {EExtension::SD_SOURCE_WIDTH, sdExtensionSpec("source-width",
                                                  {{SD_SOURCE_WIDTH_WIDTH, "width"}})}
};
