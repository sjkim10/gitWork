/****************************************************************************************
TemplateSingleTone.h
~~~~~~~~~~~~~~~~~~~~~~~~~

@ Project : Raman System
@ Date : 2024-02-24
@ Author : Piqunat
*****************************************************************************************/

#pragma once

#include <stddef.h>

/*****************************************************************************************
 *                        TemplateSingleton Template
 *****************************************************************************************/

template <typename T>
class TemplateSingleton
{
protected:
   TemplateSingleton()
   {
   }
   virtual ~TemplateSingleton()
   {
   }

public:
   static T *GetInstance()
   {
      if (m_pInstance == NULL)
         m_pInstance = new T;
      return m_pInstance;
   };

   static void DestroyInstance()
   {
      if (m_pInstance)
      {
         delete m_pInstance;
         m_pInstance = NULL;
      }
   };

private:
   static T *m_pInstance;
};

template <typename T>
T *TemplateSingleton<T>::m_pInstance = 0;

