/** ConfigurationReader.cpp
 *  Created on: 09.12.2013
 *      Author: mr.sys   */

#include <IO/ConfigurationReader.hpp>
#include <Controller/ConfigurationHandler_type.hpp>

/** Static variable definitons. */
/** for template in static varable definition
 *   https://stackoverflow.com/questions/1553854/template-static-variable */
template<> I_UserInterface * VTrans3::ConfigurationReader<attributeType>::m_p_UserInterface = NULL;
template<> TranslationControllerBase * VTrans3::ConfigurationReader<attributeType>::
  s_p_translationController = NULL;
template<> std::string VTrans3::ConfigurationReader<attributeType>::m_std_strCurrentConfigfile = "";

namespace VTrans3
{
//  template <typename attributeType>
//    ConfigurationReader<attributeType>::ConfigurationReader(TranslationControllerBase * p)
//  {
//    m_translationController = * p;
////    s_p_translationController = & r_translationController;
//  }
//
//  template <typename attributeType> ConfigurationReader<attributeType>::
//    ~ConfigurationReader<attributeType>()
//  {
//    // TODO Auto-generated destructor stub
//  }

} /* namespace VTrans3 */
