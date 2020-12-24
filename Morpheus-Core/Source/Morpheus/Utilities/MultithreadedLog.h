#pragma once

#include "Morpheus/Core/Common.h"

#include <condition_variable>
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <list>
#include <conio.h>

namespace Morpheus {

    class Log
    {
    public:
        Log() = default;
        Log(const Log&) = delete;
        Log(Log&&) = default;

        ~Log()
        {
            {
                std::lock_guard<std::mutex> lock{ mLogMutex };
                mClosing = true;
            }

            mNotificationVariable.notify_one();
            mThread.join();
        }

        template<class T>
        Log& operator <<(const T& value)
        {
            return write(std::move(value));
        }

        Log& operator <<(std::ios& (*pf)(std::ios&))
        {
            return write(std::move(pf));
        }

        Log& operator <<(std::ios_base& (*pf)(std::ios_base&))
        {
            return write(std::move(pf));
        }

        Log& operator <<(std::ostream& (*pf)(std::ostream&))
        {
            return write(std::move(pf));
        }

        Log& operator =(const Log&) = delete;
        Log& operator =(Log&&) = default;

    private:
        class LogBuf
            : public std::stringbuf
        {
        public:
            explicit LogBuf(Log& log)
                : std::stringbuf{}
                , mLog{ log }
            {
            }

            virtual ~LogBuf() = default;

        protected:
            virtual int sync() override
            {
                mLog.handleSync();
                return std::stringbuf::sync();
            }

        private:
            Log& mLog;
        };

        class LogStream
            : public std::ostream
        {
        public:
            explicit LogStream(Log& log)
                : std::ostream{ &mBuf }
                , mBuf{ log }
            {
            }

            virtual ~LogStream() = default;

            std::string getStringAndClear()
            {
                const auto string = mBuf.str();
                mBuf.str({});

                return string;
            }

        private:
            LogBuf mBuf;
        };

        struct LogEntry
        {
            std::thread::id mThreadId;
            std::function<void()> mFunctor;
        };

        std::list<LogEntry> mLogQueue;
        decltype(mLogQueue)::iterator mCurrentLog = std::end(mLogQueue);
        std::thread::id mCurrentThreadId;

        std::mutex mLogMutex;
        std::condition_variable mNotificationVariable;

        bool mClosing = false;
        bool mFlushed = false;

        LogStream mStream{ *this };

        std::thread mThread{ &Log::serialize, this };

        void handleSync()
        {
            mFlushed = true;
        }

        void serialize()
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock{ mLogMutex };
                mNotificationVariable.wait(lock, [=] {
                    return mClosing || mCurrentLog != std::end(mLogQueue);
                });

                if (mClosing)
                {
                    while (!mLogQueue.empty())
                    {
                        if (mCurrentLog == std::end(mLogQueue))
                        {
                            mCurrentLog = std::begin(mLogQueue);
                            mCurrentThreadId = mCurrentLog->mThreadId;
                        }

                        while (mCurrentLog != std::end(mLogQueue))
                        {
                            if (mCurrentThreadId == mCurrentLog->mThreadId)
                            {
                                mCurrentLog->mFunctor();
                                mCurrentLog = mLogQueue.erase(mCurrentLog);
                            }
                            else
                            {
                                ++mCurrentLog;
                            }
                        }
                    }

                    flush();
                    break;
                }

                do {
                    mCurrentLog->mFunctor();

                    if (mFlushed)
                    {
                        flush();

                        mLogQueue.erase(mCurrentLog);
                        mCurrentLog = std::begin(mLogQueue);

                        if (mCurrentLog == std::end(mLogQueue))
                        {
                            mCurrentThreadId = {};
                            break;
                        }

                        mCurrentThreadId = mCurrentLog->mThreadId;
                    }
                    else
                    {
                        mCurrentLog = std::find_if(mLogQueue.erase(mCurrentLog), std::end(mLogQueue), [=](const auto& entry) {
                            return mCurrentThreadId == entry.mThreadId;
                        });
                    }
                } while (mCurrentLog != std::end(mLogQueue) && mCurrentThreadId == mCurrentLog->mThreadId);
            }
        }

        template<class T>
        Log& write(T value)
        {
            const auto id = std::this_thread::get_id();

            std::lock_guard<std::mutex> lock{ mLogMutex };
            mLogQueue.emplace_back(LogEntry{ id,[=, value = std::move(value)] {
                mStream << value;
            } });

            if (mCurrentThreadId == std::thread::id{})
            {
                mCurrentLog = std::begin(mLogQueue);
                mCurrentThreadId = mCurrentLog->mThreadId;
                mNotificationVariable.notify_one();
            }
            else if (mCurrentThreadId == id)
            {
                if (mCurrentLog == std::end(mLogQueue))
                    mCurrentLog = std::prev(std::end(mLogQueue));

                mNotificationVariable.notify_one();
            }

            return *this;
        }

        void Color(int color)
        {
            //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        }

        void flush()
        {
            std::string Message = mStream.getStringAndClear();

            if (Message.substr(0, 1) == "§") {
                std::stringstream code(Message.substr(1, 2));
                int x = 0; code >> x;
                Color(x);
                std::cout << Message.substr(3, Message.length());
                Color(12);
            }         
            
            else std::cout << Message;

            mFlushed = false;
        }
    };

} 