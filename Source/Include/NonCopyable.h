#pragma once

class NonCopyable
{
protected:
  NonCopyable() {};
  ~NonCopyable() {};
  NonCopyable(const NonCopyable&);
  NonCopyable& operator=(const NonCopyable&);
};