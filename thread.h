#ifndef DOWNLOAD_THREAD_H
#define DOWNLOAD_THREAD_H

namespace torrent
{
    class thread
    {
    private:
        int id;
        int peer_id;

    public:
        thread(/* args */);
        ~thread();
    };

}

#endif // !DOWNLOAD_THREAD_H