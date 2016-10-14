/** @file
 *
 * @ingroup spatdiflib
 *
 * @brief
 *
 * @details
 *
 * @authors Chikashi Miyama
 *
 * @copyright Copyright (C) 2013 - 2014 Zurich University of the Arts, Institute for Computer Music and Sound Technology @n
 * This code is licensed under the terms of the "New BSD License" @n
 * http://creativecommons.org/licenses/BSD/
 */

#pragma once
#include "sdDescriptor.h"
#include "sdValue.h"

template <EDescriptor D>
class sdData :public sdDescriptor , public sdValue<D>{
    
public:
    sdData(const typename sdDescriptorSpec<D>::type &value): sdDescriptor(D),sdValue<D>(value){}
};

