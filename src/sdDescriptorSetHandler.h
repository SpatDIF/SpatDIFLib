//
//  sdDescriptorSetHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 26/08/16.
//  Copyright © 2016 ICST. All rights reserved.
//

#prgma once
#include <unordered_map>
#include "sdDescriptor.h"

class sdDescriptorSetHandler{
    std::unordered_map<EExtension, std::unordered_map<std::string, std::unique_ptr<sdProtoDescriptorSet>> descriptorSets;
    

    
    
};


