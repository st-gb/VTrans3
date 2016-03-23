/*
 * VocAttributeDefintionHandler.hpp
 *
 *  Created on: 22.12.2013
 *      Author: mr.sys
 */

#ifndef VOCATTRIBUTEDEFINTIONHANDLER_HPP_
#define VOCATTRIBUTEDEFINTIONHANDLER_HPP_

#include <mxml.h> //mxml_node_t, mxml_sax_event_t

class TranslationControllerBase;

namespace VTrans3
{
  namespace MiniXML
  {

    class VocAttributeDefintionHandler
    {
    public:
      static TranslationControllerBase * s_p_translationController;
      VocAttributeDefintionHandler(TranslationControllerBase & r_translationController);
      virtual
      ~VocAttributeDefintionHandler();
    };

    namespace VocAttributeDefintionFile
    {
      /** see http://www.msweet.org/documentation/project3/Mini-XML.html#4_7 */
      void sax_cb(
        mxml_node_t * node,
        mxml_sax_event_t event,
        void * data
        );
    }
  } /* namespace MiniXML */
} /* namespace VTrans3 */

#endif /* VOCATTRIBUTEDEFINTIONHANDLER_HPP_ */
