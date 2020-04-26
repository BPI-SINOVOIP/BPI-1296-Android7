////////////////////////////////////////////////////////////////////////////////
/// Command proccesor interface.<BR>
///
/// This interface will be called by CommandQueue to process commands.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_COMMAND_PROCESSOR__
#define __INCLUDED_COMMAND_PROCESSOR__

namespace rtk    {
namespace tools  {

template <typename T> class CommandProcessor
{
    public:
        /// Execute command
        ///
        /// @param cmd The command wants to be executed.
        ///
        /// @return true --> Success. false--> Failed.
        ///
        virtual bool Execute(T cmd) = 0;
        
        /// Remove command.<BR>
        /// This function will be called when the command is removed from CommandQueue.<BR>
        /// This callback is useful when the T is allocated dynamically.<BR>
        /// It gives you a chance to free the allocated T.<BR>
        ///
        /// @param cmd The command wants to be removed from CommandQueue.
        ///
        virtual void Remove(T cmd) { ; };
        
    public:
        virtual ~CommandProcessor() { ; };
};

}; // namespace tools
}; // namespace rtk

#endif
