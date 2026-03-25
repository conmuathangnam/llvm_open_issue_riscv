namespace internal {

template <class T>
struct Identity {
  using type = T;
};

template <class T, template <class> class Ignored>
using ResolveIdentity = typename Identity<T>::type;

template <class T>
using SimpleAlias = T;

template <class T, class Ignored>
using PointerAlias = ResolveIdentity<T*, SimpleAlias>;

template <class T, class U, class V>
using CharPtrAlias = char*;

template <class T>
using TypeAlias = T;

template <class Ignored, class T>
using IdentityAlias = ResolveIdentity<T, TypeAlias>;

template <class T>
struct UnderlyingType {
  using type = typename T::size_type;
};

template <class T>
struct Traits {
  using source = T;
  using size_type = typename UnderlyingType<source>::type;
  using pointer_type = IdentityAlias<source, size_type>;
};

char global_buffer;

struct SizeProvider {
  typedef decltype(static_cast<int*>(nullptr) - static_cast<int*>(nullptr)) size_type;
};

template <class T>
struct BufferWrapper {
  using char_type = char;
  using size_provider = SizeProvider;
  using pointer_alias = PointerAlias<char_type, size_provider>;
  using char_ptr = CharPtrAlias<char_type, pointer_alias, size_provider>;

  struct InnerStorage {
    pointer_alias ptr;
  };

  struct Container {
    InnerStorage storage;
  } container;

  BufferWrapper(typename Traits<size_provider>::pointer_type size, char val) {
    pointer_alias initial_ptr(&global_buffer);
    container.storage.ptr = initial_ptr;
    char* target = &global_buffer;
    for (; size; ++target, --size) *target = 0;
  }

  char_type* get_data() {
    char_ptr data = is_valid() ? get_storage_ptr() : 0;
    return data;
  }

  bool is_valid() { return true; }
  char_ptr get_storage_ptr() { return container.storage.ptr; }
};

using DefaultBuffer = BufferWrapper<char>;

}  // namespace internal

struct StringRef {
  template <typename T>
  StringRef(internal::BufferWrapper<T> wrapper) : StringRef(wrapper.get_data(), 0, {}) {}

  StringRef(long size) : length(size) {}

  long get_length() const { return length; }
  char* get_data() const { return data; }

  operator internal::DefaultBuffer();

  StringRef(char* ptr, long len, int) : data(ptr) {}

  char* data;
  long length;
};

bool operator==(StringRef lhs, StringRef rhs) {
  long len = lhs.get_length();
  char* data = rhs.get_data();
  // Using __builtin_memcmp with a size that LLVM will vectorize (16 bytes).
  return __builtin_memcmp(0, data, len) == 0;
}

void trigger_crash(StringRef input) {
  StringRef ref16(16);
  internal::DefaultBuffer buffer16(16, '\0');
  // Comparing a 16-byte object with vectorization enabled triggers the crash.
  if (ref16 != buffer16) {
    internal::DefaultBuffer unused(input);
  }
}
