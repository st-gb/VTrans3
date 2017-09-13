/** TUchemnitzDictEngWord1stReader.cpp
 *  Created on: Dec, 2013
 *      Author: Stefan  */

#include "BinarySearchInDictFile.hpp"
//#include "FileSystem/File/GetAbsoluteFilePath.hpp"
#include <preprocessor_macros/logging_preprocessor_macros.h> //LOGN_DEBUG(...)
#include <OperatingSystem/GetLastErrorCode.hpp>
#include <FileSystem/File/FileReadException.hpp>
#include <FileSystem/GetAbsolutePath.hpp>
#include <IO/dictionary/DictionaryFileAccessException.hpp>
#include <IO/dictionary/OpenDictFileException.hpp>//class OpenDictFileException

namespace DictionaryReader
{
  namespace TUchemnitz {
  namespace EngWordSorted1st
  {
    namespace EachAttributeInSingleLine
    {
    BinarySearchInDictFile::BinarySearchInDictFile(
        IVocabularyInMainMem & vocaccess)
      : BinarySearchInDictData(vocaccess)
    {
    }

    BinarySearchInDictFile::~BinarySearchInDictFile()
    {
      LOGN_DEBUG("end")
    }

    /** Do not load anything into memory. */
    bool BinarySearchInDictFile::open(const std::string & std_strDictFilePath )
    {
      LOGN_DEBUG("Opening file " << std_strDictFilePath)
//      m_englishDictionary.open(std_strDictFilePath.c_str(),
//        std::ios_base::in | std::ios_base::binary );
      enum I_File::OpenError openError = m_dictFile.OpenA(
        std_strDictFilePath.c_str(),
        I_File::readOnly
        );
//      bool dictFileIsOpen = m_englishDictionary.is_open();
      bool dictFileIsOpen = openError == I_File::success;
      if( /*dictFileIsOpen*/ openError == I_File::success )
      {
        LOGN_DEBUG("Dict file is open")
//        m_englishDictionary.seekg(0, std::ios_base::end);
//        m_fileSizeInBytes = m_englishDictionary.tellg();
        m_fileSizeInBytes = m_dictFile.GetFileSizeInBytes();
        if( m_fileSizeInBytes == -1 )
        {
          const DWORD lastError = OperatingSystem::GetLastErrorCode();
          std::string absoluteDictFilePath = FileSystem::GetAbsolutePathA(
            std_strDictFilePath.c_str());
          DictionaryFileAccessException dictionaryFileAccessException(
            DictionaryFileAccessException::getFileSize, lastError,
            absoluteDictFilePath.c_str() );
  	      LOGN_ERROR("error getting dictionary file size->throwing an exception")
          throw dictionaryFileAccessException;
        }
//        m_englishDictionary.seekg(0, std::ios_base::beg);
//        m_dictFile.Seek(0);
      }
      else //Or throw enum I_File::OpenError openError
      {
        std::string absoluteDictFilePath = FileSystem::GetAbsolutePathA(
          std_strDictFilePath.c_str());
	      LOGN_ERROR("error loading dictionary \"" << absoluteDictFilePath << "\"->throwing an exception")
//        const DWORD lastError = OperatingSystem::GetLastErrorCode();
        throw VTrans3::OpenDictFileException(openError);
      }
      return dictFileIsOpen;
    }

    //          switch(ch)
    //          {
    //            case ENGLISH_GERMAN_SEPERATOR_CHAR :
    //            case '('
    //            case SYNONYM_SEPERATOR_CHAR :
    //            case SAME_VOCABLE_SEPERATOR_CHAR :
    //              englishGermanSeperatorCharOccurred = true;
    //              word[charIndex ++] = '\0';
    //              break;
    //            case '\r':
    //  //          case '\n':
    //              newLine = true;
    //              break;
    //              if(englishGermanSeperatorCharOccurred)
    //              char * pch = new char[charIndex];
    //              memcpy(pch, word, charIndex + 1);
    //              m_p_vocabularyAccess->InsertAsKeyAndAddVocabularyAttributes(
    //                word,
    //                );
    //              charIndex = 0;
    //              break;
    //            default:
    //              word[charIndex ++] = ch;
    //              break;
    //          }
    //          if(englishGermanSeperatorCharOccurred)
    //          {
    //            psv.at(0);
    //          }
    //          i = m_arbyCharValueToArrayIndex[ch];
    //          p_BytePosAndNextChar = m_BytePosAndNextChar[i];
    //          p_BytePosAndNextChar->m_bytePos = filePosInByte;
    //          ++ filePosInByte;
    //          i = m_englishDictionary.get();
    //        }

    int BinarySearchInDictFile::GetNextByte()
    {
      int readByteReturnValue;
      try
      {
        readByteReturnValue = m_dictFile.ReadByte();
      }
      catch( const FileReadException & fileReadException ) {
        /** Re-throw a DictionaryFileAccessException to indicate that the 
         *  exception was caused while accessing a _dictionary_ file. */
        DictionaryFileAccessException dictionaryFileAccessException(
          fileReadException,
          DictionaryFileAccessException::read
//          lastOSerrorCode,
//            m_dictFile.
//          filePath
          );
        throw dictionaryFileAccessException;
      }
      return readByteReturnValue;
    }

    I_File::ReadResult BinarySearchInDictFile::ReadByteBuffer(
      uint8_t buffer [],
      fastestUnsignedDataType & numBytes)
    {
      I_File::ReadResult readResult;
	    readResult = m_dictFile.Read(buffer, numBytes, numBytes);
	    if( readResult != I_File::successfullyRead )
      {
	      if( readResult != I_File::readLessThanIntended )
        {
          FileReadException fileReadException(readResult, errno,
            m_dictFile.GetFilePathA().c_str() );
          /** Throw a DictionaryFileAccessException to indicate that the
           *  exception was caused while accessing a _dictionary_ file. */
          DictionaryFileAccessException dictionaryFileAccessException(
            fileReadException,
            DictionaryFileAccessException::read
            );
          throw dictionaryFileAccessException;
        }
      }
      return readResult;
		}

    int BinarySearchInDictFile::GetCurrentDictDataOffset()
    {
      LOGN_DEBUG("begin")
      I_File::file_pointer_type currOffset = m_dictFile.GetCurrentFilePointerPosition();
      if( currOffset == -1 )
      {
        const DWORD lastOSerrorCode = OperatingSystem::GetLastErrorCode();
        const char * const filePath = m_dictFile.GetFilePathA().c_str();
        DictionaryFileAccessException dictionaryFileAccessException(
          DictionaryFileAccessException::getCurrentFilePointerPosition,
          lastOSerrorCode,
//            m_dictFile.
          filePath
          );
        const std::string errorMessage = dictionaryFileAccessException.GetErrorMessageA();
        LOGN_ERROR(//"error seeking file pointer to offset " << byteOffset
          errorMessage
          );
        throw dictionaryFileAccessException;
      }
      LOGN_DEBUG("return " << currOffset)
      return currOffset;
    }

    bool BinarySearchInDictFile::SetDictDataOffset(
      const fastestUnsignedDataType byteOffset)
    {
      LOGN_DEBUG("begin seek to offset " << byteOffset )
		const enum I_File::SeekResult seekResult = m_dictFile.SeekFilePointerPosition(byteOffset);
      const bool streamIsGood = seekResult == I_File::successfullySeeked;
      if( seekResult != I_File::successfullySeeked )
      {
        const DWORD lastOSerrorCode = OperatingSystem::GetLastErrorCode();
        const char * const filePath = m_dictFile.GetFilePathA().c_str();
        DictionaryFileAccessException dictionaryFileAccessException(
          DictionaryFileAccessException::seek,
          lastOSerrorCode,
//            m_dictFile.
          filePath,
          byteOffset
          );
        const std::string errorMessage = dictionaryFileAccessException.GetErrorMessageA();
        LOGN_ERROR(//"error seeking file pointer to offset " << byteOffset
          errorMessage
          );
        throw dictionaryFileAccessException;
      }
      LOGN_DEBUG("return " << streamIsGood << " (=stream is " <<
        (streamIsGood ? "good" : "bad") )
      return streamIsGood;
    }

    //TODO idea: because words are sorted lexicographically:
    // store english words: at 1st store number of 1st identical
    // characters:
    //  antimatter
    //  antimephitic
    // ->if antimatter is the 1st word inside the dictionary, store:
    // "antimatter", "5"(# 1st identical characters),"ephitic"
    void BinarySearchInDictFile::read()
    {
//  //    m_bytePos[letter] = bytePos;
//      unsigned filePosInByte = 0;
//      char ch;
//      int i;
//      i = m_englishDictionary.get();
//      char word[100];
////      BytePosAndNextChar * p_BytePosAndNextChar;
//      fastestUnsignedDataType charIndex = 0;
//      bool newLine = false;
//      bool englishGermanSeperatorCharOccurred = false;
//      while(i != eof)
//      {
//        ch = i;
//        switch(ch)
//        {
//          case ENGLISH_GERMAN_SEPERATOR_CHAR :
//            englishGermanSeperatorCharOccurred = true;
//          case '\r':
//          case '\n':
//            newLine = true;
//          case SYNONYM_SEPERATOR_CHAR :
//          case SAME_VOCABLE_SEPERATOR_CHAR :
//            if(englishGermanSeperatorCharOccurred)
//            word[charIndex] = '\0';
////            char * pch = new char[charIndex];
//            memcpy(pch, word, charIndex + 1);
////            m_p_vocabularyAccess->InsertAsKeyAndAddVocabularyAttributes(
////              word,
////              );
//            charIndex = 0;
//            break;
//          default:
//            word[charIndex ++] = ch;
//            break;
//        }
//        i = m_arbyCharValueToArrayIndex[ch];
//        p_BytePosAndNextChar = m_BytePosAndNextChar[i];
//        p_BytePosAndNextChar->m_bytePos = filePosInByte;
//        ++ filePosInByte;
//        i = m_englishDictionary.get();
//      }
    }

//    /** Inserts the string as a key value to the container and adds the string
//     *  as an attribute value.*/
//    VocabularyAndTranslation * InsertAsKeyAndAddVocabularyAttributes(
//      const char * ar_chWordBegin,
//      unsigned stringLen,
//      enum EnglishWord::English_word_class word_class
//      )
//    {
//      /*void * p_v =*/ return Insert(ar_chWordBegin, stringLen, word_class, NULL);
//  //      AddVocabularyAttributes( word_class, p_v);
//    }
//
//    /** Save memory by referring to existing voc attributes. */
//    VocabularyAndTranslation * InsertAsKeyAndReferToExistingVocData(
//      enum EnglishWord::English_word_class word_class,
//      const char * ar_chWordBegin,
//      unsigned stringLen,
//      const VocabularyAndTranslation * const p_vocandtranslAllocated
//      )
//    {
//      VocabularyAndTranslation * p_vocandtransl =
//        Insert(ar_chWordBegin, stringLen, word_class, NULL);
//      if( p_vocandtransl )
//      {
//        p_vocandtransl->PointToAttributeData(p_vocandtranslAllocated);
//        p_vocandtransl->m_englishWordClass = word_class;
//      }
//      return p_vocandtransl;
//    };
  }
  }
  }
} /* namespace DictionaryReader */
