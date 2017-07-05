#include <nds3/nds.h>

/** @brief Class that shows how the PVAction type work.
 *
 * @warning For the purpose of the example the declaration of the class is in the same
 *          file as its implementation. In a real life situation the declaration should be
 *          in a separate header file.
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few manadatory parameters and should register the root node via Node::initialize().
 */

class RPCSquare
{
public:
  /**
   * @brief Constructor.
   *
   * @param factory    the control system factory that requested the creation of the device
   * @param device     the name given to the device
   * @param parameters optional parameters passed to the device
   */
  RPCSquare(nds::Factory& factory, const std::string& device,
            const nds::namedParameters_t& parameters);


  /**
   * @brief RPC function. In this case it will square the input.
   *
   * @param[in] timestamp The timestamp.
   * @param[in] value The input value.
   */
  void square(const timespec& timestamp, const std::int32_t& value);

  /**
   * @brief Initial state for the PVAction.
   *
   * @param[out] timestamp The timestamp.
   * @param[out] value The initial state.
   */
  void init(timespec* timestamp, std::int32_t* value);

private:

  /**
   * @brief The RPC PV
   */
  nds::PVAction squarePV;
};

RPCSquare::RPCSquare(nds::Factory& factory, const std::string& device,
                     const nds::namedParameters_t& parameters)
{
  nds::Port rootNode(device);

  squarePV = nds::PVAction(
    "Square", std::bind(&RPCSquare::square, this,
                             std::placeholders::_1, std::placeholders::_2),
    std::bind(&RPCSquare::init, this, std::placeholders::_1,
              std::placeholders::_2));
  squarePV.setDescription("Square any integer");
  rootNode.addChild(squarePV);

  rootNode.initialize(this, factory);
}

void
RPCSquare::square(const timespec& timestamp, const std::int32_t& value)
{
  squarePV.setValueBack(timestamp, value*value);
}

void
RPCSquare::init(timespec* timestamp, std::int32_t* value)
{
  *value = 0;
}

NDS_DEFINE_DRIVER(RPCSquare, RPCSquare)
