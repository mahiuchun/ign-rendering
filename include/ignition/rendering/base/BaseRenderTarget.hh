/*
 * Copyright (C) 2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef IGNITION_RENDERING_BASE_BASERENDERTARGET_HH_
#define IGNITION_RENDERING_BASE_BASERENDERTARGET_HH_

#include "ignition/rendering/RenderTarget.hh"
#include "ignition/rendering/base/BaseRenderTypes.hh"

namespace ignition
{
  namespace rendering
  {
    template <class T>
    class IGNITION_VISIBLE BaseRenderTarget :
      public virtual RenderTarget,
      public virtual T
    {
      public: BaseRenderTarget();

      public: virtual ~BaseRenderTarget();

      public: virtual void PreRender();

      public: virtual unsigned int Width() const;

      public: virtual void SetWidth(unsigned int _width);

      public: virtual unsigned int Height() const;

      public: virtual void SetHeight(unsigned int _height);

      protected: virtual void Rebuild();

      protected: virtual void RebuildImpl() = 0;

      protected: bool targetDirty = true;

      protected: unsigned int width = 0u;

      protected: unsigned int height = 0u;
    };

    template <class T>
    class IGNITION_VISIBLE BaseRenderTexture :
      public virtual RenderTexture,
      public virtual T
    {
      public: BaseRenderTexture();

      public: virtual ~BaseRenderTexture();

      public: virtual PixelFormat Format() const;

      public: virtual void SetFormat(PixelFormat _format);

      protected: PixelFormat format;
    };

    template <class T>
    class IGNITION_VISIBLE BaseRenderWindow :
      public virtual RenderWindow,
      public virtual T
    {
      public: BaseRenderWindow();

      public: virtual ~BaseRenderWindow();

      public: virtual void OnResize(const unsigned int _width,
                  const unsigned int _height);

      public: virtual void OnMove();

      std::string handle;
    };

    //////////////////////////////////////////////////
    // BaseRenderTarget
    //////////////////////////////////////////////////
    template <class T>
    BaseRenderTarget<T>::BaseRenderTarget()
    {
    }

    //////////////////////////////////////////////////
    template <class T>
    BaseRenderTarget<T>::~BaseRenderTarget()
    {
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderTarget<T>::PreRender()
    {
      T::PreRender();
      this->Rebuild();
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderTarget<T>::Rebuild()
    {
      if (this->targetDirty)
      {
        this->RebuildImpl();
        this->targetDirty = false;
      }
    }

    //////////////////////////////////////////////////
    template <class T>
    unsigned int BaseRenderTarget<T>::Width() const
    {
      return this->width;
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderTarget<T>::SetWidth(unsigned int _width)
    {
      this->width = _width;
      this->targetDirty = true;
    }

    //////////////////////////////////////////////////
    template <class T>
    unsigned int BaseRenderTarget<T>::Height() const
    {
      return this->height;
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderTarget<T>::SetHeight(unsigned int _height)
    {
      this->height = _height;
      this->targetDirty = true;
    }


    //////////////////////////////////////////////////
    // BaseRenderTexture
    //////////////////////////////////////////////////
    template <class T>
    BaseRenderTexture<T>::BaseRenderTexture() :
      format(PF_UNKNOWN)
    {
    }

    //////////////////////////////////////////////////
    template <class T>
    BaseRenderTexture<T>::~BaseRenderTexture()
    {
    }

    //////////////////////////////////////////////////
    template <class T>
    PixelFormat BaseRenderTexture<T>::Format() const
    {
      return this->format;
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderTexture<T>::SetFormat(PixelFormat _format)
    {
      this->format = PixelUtil::Sanitize(_format);
      this->targetDirty = true;
    }

    //////////////////////////////////////////////////
    // BaseRenderWindow
    //////////////////////////////////////////////////
    template <class T>
    BaseRenderWindow<T>::BaseRenderWindow()
    {
    }

    //////////////////////////////////////////////////
    template <class T>
    BaseRenderWindow<T>::~BaseRenderWindow()
    {
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderWindow<T>::OnResize(const unsigned int _width,
        const unsigned int _height)
    {
      this->width = _width;
      this->height = _height;
      this->targetDirty = true;
    }

    //////////////////////////////////////////////////
    template <class T>
    void BaseRenderWindow<T>::OnMove()
    {
      this->targetDirty = true;
    }

  }
}
#endif
