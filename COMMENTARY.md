In MO2, each mod has its own folder, because the mechanism is that on startup MO2 replace game content with mod content.

In L4D2, each addon has its archive only in the form of vpk. No easy access to these vpks. The mechanism is that prior to startup we must link these files to the game's folder. L4D2 has a basic addon system already, this one's only an extension, not a substitution.

In MO2, each mod is detected and scanned by the meta.ini file in each folder. 

In the Unity game editor, each file has an accompanying .meta file. But this's assuming that we interface with the game editor's filesystem interface more than with our native file manager (which they were wrong, thus I fucking hate unity).

In L4D2, I plan to use one addonlist file for the whole addons_dock folder. This file will be updated accordingly, perhaps as a callback after each operation?

---

Now that I think about it... storing all collected data in a text file / folder isn't a great idea. Storing all thumbnails in addons_dock/thumbnails is not good. Storing all metadata in addondict.json might work. I haven't finished implementing that. Technically, what you see in the json will be what's on window, nice. Generally, there are benefits to managing everything in one list file. Access time will be short.

But then I came across something cooler. If you give an addon its own folder, you can attach there its own thumbnail jpg and metadata json. From a creator standpoint, it narrows down your focus. You don't have to edit addonsdict.json or press Import every time you want to test out a local addon. From a user standpoint, addons are now sharable as folders, and you can go back to vpks without being destructive as the vpk is just inside the folder. This is especially useful when an addon is split into multiple parts or when package raw data is split from their header file. And from a dev standpoint, each addon is now a file/directory unit and info can be gathered iteratively on the spot; previously, you have to search for the respective info based on the reference in addondict.json, a two-step process.

Also I just realized this but theres a cool benefit to having the program's own version of addoninfo.txt. It will still use VDF. So, won't it be just a duplicate? Yes. Specifically, it's an extension. The program mainly interfaces with the exposed txt, the archived one will only be sync on demand. It's important that it is like this so that access time can be low. It's a sacrifice of memory for speed.

Of course, there must be a means of organization. We will have addonlist.txt. It will be loaded on startup and be updated on reorganization. Priority data is now stored in these files only, not even in addoninfo.txt. Of course, because we're using GUI there will also be extended elements like groupable separators. Notes...

Actually, we could use csv if its faster.

---

The philosophy here is that the user sets up addons by themselves, just like how I have to download mp3s from youtube so as to play them in Rhythmbox. Except download and sync tools are provided so as to incentivize users to edit in the first place.

Also I can see addoninfo without internet access ;(