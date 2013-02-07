///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/texture_cube.inl
/// @date 2011-04-06 / 2012-12-12
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

namespace gli
{
	inline textureCube::textureCube() :
		View(0, 0, 0, 0, 0, 0),
		Format(FORMAT_NULL)
	{}

	inline textureCube::textureCube
	(
		size_type const & Faces,
		size_type const & Levels,
		format_type const & Format,
		dimensions_type const & Dimensions
	) :
		Storage(
			1,
			Faces,
			Levels,
			Format,
			storage::dimensions_type(Dimensions, 1)),
		View(0, 0, 0, Faces - 1, 0, Levels - 1),
		Format(Format)
	{}

	inline textureCube::textureCube
	(
		size_type const & Faces,
		format_type const & Format,
		dimensions_type const & Dimensions
	) :
		Storage(
			1,
			Faces,
			size_type(glm::log2(int(glm::max(Dimensions.x, Dimensions.y))) + 1),
			Format,
			storage::dimensions_type(Dimensions, 1)),
		View(
			0, 0, 
			0, Faces - 1, 
			0, size_type(glm::log2(int(glm::max(Dimensions.x, Dimensions.y))))),
		Format(Format)
	{}

	inline textureCube::textureCube
	(
		storage const & Storage
	) :
		Storage(Storage),
		View(0, 0, 0, Storage.faces() - 1, 0, Storage.levels() - 1),
		Format(Storage.format())
	{}

	inline textureCube::textureCube
	(
		storage const & Storage,
		format_type const & Format,
		gli::view const & View
	) :
		Storage(Storage),
		View(View),
		Format(Format)
	{}

	inline textureCube::textureCube
	(
		textureCube const & Texture,
		size_type const & BaseFace,
		size_type const & MaxFace,
		size_type const & BaseLevel,
		size_type const & MaxLevel
	) :
		Storage(Texture.Storage),
		View(
			Texture.view().BaseLayer,
			Texture.view().MaxLayer,
			Texture.view().BaseFace + BaseFace,
			Texture.view().BaseFace + MaxFace,
			Texture.view().BaseLevel + BaseLevel,
			Texture.view().BaseLevel + MaxLevel),
		Format(Texture.format())
	{}

	inline textureCube::textureCube
	(
		textureCubeArray const & Texture,
		size_type const & BaseLayer,
		size_type const & BaseFace,
		size_type const & MaxFace,
		size_type const & BaseLevel,
		size_type const & MaxLevel
	) :
		Storage(Texture),
		View(
			Texture.view().BaseLayer + BaseLayer,
			Texture.view().BaseLayer + BaseLayer,
			Texture.view().BaseFace + BaseFace,
			Texture.view().BaseFace + MaxFace,
			Texture.view().BaseLevel + BaseLevel,
			Texture.view().BaseLevel + MaxLevel),
		Format(Texture.format())
	{}

	inline textureCube::textureCube
	(
		texture2D const & Texture,
		size_type const & BaseLevel,
		size_type const & MaxLevel
	) :
		Storage(Texture),
		View(
			Texture.view().BaseLayer,
			Texture.view().MaxLayer,
			Texture.view().BaseFace,
			Texture.view().MaxFace,
			Texture.view().BaseLevel + BaseLevel,
			Texture.view().BaseLevel + MaxLevel),
		Format(Texture.format())
	{}

	inline textureCube::operator storage() const
	{
		return this->Storage;
	}

	inline texture2D textureCube::operator[] (size_type const & Face) const
	{
		assert(Face < this->faces());

		return texture2D(
			this->Storage,
			this->format(),
			gli::view(
				this->View.BaseLayer,
				this->View.MaxLayer,
				this->View.BaseFace + Face,
				this->View.BaseFace + Face,
				this->View.BaseLevel,
				this->View.MaxLevel));
	}

	inline bool textureCube::empty() const
	{
		return this->Storage.empty();
	}

	inline textureCube::dimensions_type textureCube::dimensions() const
	{
		return textureCube::dimensions_type(this->Storage.dimensions(this->View.BaseLevel));
	}

	inline textureCube::format_type textureCube::format() const
	{
		return this->Format;
	}

	inline textureCube::size_type textureCube::layers() const
	{
		return 1;
	}
	
	inline textureCube::size_type textureCube::faces() const
	{
		return this->View.MaxFace - this->View.BaseFace + 1;
	}
	
	inline textureCube::size_type textureCube::levels() const
	{
		return this->View.MaxLevel - this->View.BaseLevel + 1;
	}

	inline view const & textureCube::view() const
	{
		return this->View;
	}

	inline textureCube::size_type textureCube::size() const
	{
		assert(!this->empty());

		return this->Storage.layerSize(
			this->View.BaseFace, this->View.MaxFace,
			this->View.BaseLevel, this->View.MaxLevel);
	}

	inline void * textureCube::data()
	{
		assert(!this->empty());

		size_type const offset = detail::imageAddressing(
			this->Storage, this->View.BaseLayer, this->View.BaseFace, this->View.BaseLevel);

		return this->Storage.data() + offset;
	}

	inline void const * textureCube::data() const
	{
		assert(!this->empty());
		
		size_type const offset = detail::imageAddressing(
			this->Storage, this->View.BaseLayer, this->View.BaseFace, this->View.BaseLevel);

		return this->Storage.data() + offset;
	}

	template <typename genType>
	inline textureCube::size_type textureCube::size() const
	{
		assert(!this->empty());
		assert(sizeof(genType) <= this->Storage.blockSize());

		return this->size() / sizeof(genType);
	}

	template <typename genType>
	inline genType * textureCube::data()
	{
		assert(!this->empty());
		assert(this->Storage.blockSize() >= sizeof(genType));

		return reinterpret_cast<genType *>(this->data());
	}

	template <typename genType>
	inline genType const * textureCube::data() const
	{
		assert(!this->empty());
		assert(this->Storage.blockSize() >= sizeof(genType));

		return reinterpret_cast<genType const *>(this->data());
	}

	inline void textureCube::clear()
	{
		for(size_type Face = 0; Face < this->faces(); ++Face)
			(*this)[Face].clear();
	}

	template <typename genType>
	inline void textureCube::clear(genType const & Texel)
	{
		assert(this->Storage.blockSize() == sizeof(genType));

		for(size_type Face = 0; Face < this->faces(); ++Face)
			(*this)[Face].clear<genType>(Texel);
	}
}//namespace gli
