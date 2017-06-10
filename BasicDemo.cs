using UnityEngine;
using System.Collections;
using UnityEngine.UI;

using TechTweaking.Bluetooth;

public class BasicDemo : MonoBehaviour {

	private  BluetoothDevice device;
	public TextMesh statusText;
	public TextMesh logsOnScreen; 
	public int count = 0;
	public int val1;
	public int val2;
	public GameObject cube;
	public int theta;
	public double t;
	public Transform Cam_transform;  


	// Use this for initialization
	void Awake () {

		Cam_transform.position = new Vector3 (0, 0, 0);

		BluetoothAdapter.enableBluetooth();//Force Enabling Bluetooth


		device = new BluetoothDevice();

		/*
		 * We need to identefy the device either by its MAC Adress or Name (NOT BOTH! it will use only one of them to identefy your device).
		 *
		 *---------- MacAdress property
		 * Using the MAC adress is the best choice because the device doesn't have to be paired/bonded!
		 * 
		 * ----------Name property
		 * Identefy a device by its name using the Property 'BluetoothDevice.Name' require the remote device to be paired
		 * but you can try to alter the parameter 'allowDiscovery' of the Connect(int attempts, int time, bool allowDiscovery) method. 
		 * allowDiscovery will start a heavy discovery process (if the remote device weren't paired). This will take time 12 to 25 seconds.
		 * So it's better to use the 'BluetoothDevice.MacAdress' property. It doesn't need previuos pairing/bonding.
		 */


		device.Name = "HC-05";
		//device.MacAddress = "XX:XX:XX:XX:XX:XX";



		/*
		 * 10 equals the char '\n' which is a "new Line" in Ascci representation, 
		 * so the read() method will retun a packet that was ended by the byte 10. simply read() will read lines.
		 * If you don't use the setEndByte() method, device.read() will return any available data (line or not), then you can order them as you want.
		 */
		device.setEndByte (0);


		/*
		 * The ManageConnection Coroutine will start when the device is ready for reading.
		 */
		connect ();
		device.ReadingCoroutine = ManageConnection;


		/*
		 *  Note: The library will fill the properties device.Name and device.MacAdress with the right data after succesfully connecting.
		 * 
		 *  Moreover, any BluetoothDevice instance returned by a method or event of this library will have both properties (Name & MacAdress) filled with the right data
		 */
	}

	public void connect() {
		statusText.text = "Status : ...";

		/*
		 * Notice that there're more than one connect() method, check out the docs to read about them.
		 * a simple device.connect() is equivalent to connect(10, 1000, false) which will make 10 connection attempts
		 * before failing completly, each attempt will cost at least 1 second.
		 * -----------
		 * To alter that  check out the following methods in the docs :
		 * connect (int attempts, int time, bool allowDiscovery) 
		 * normal_connect (bool isBrutal, bool isSecure)
		 */
		device.connect();

	}

	public void disconnect() {
		device.close();
	}


	//############### Reading Data  #####################
	//Please note that you don't have to use this Couroutienes/IEnumerator, you can just put your code in the Update() method
	IEnumerator  ManageConnection (BluetoothDevice device)
	{
		statusText.text = "Status : Connected & Can read";
		while (device.IsConnected && device.IsReading) {
			count = (count + 1) % 500;
			statusText.text = "Status : Connected & Can read"+count;
			//polll all available packets
			BtPackets packets = device.readAllPackets();

			if (packets != null) {

				/*
				 * parse packets, packets are ordered by indecies (0,1,2,3 ... N),
				 * where Nth packet is the latest packet and 0th is the oldest/first arrived packet.
				 * 
				 * Since this while loop is looping one time per frame, we only need the Nth(the latest potentiometer/joystick position in this frame).
				 * 
				 */
				int N = packets.Count - 1; 
				//packets.Buffer contains all the needed packets plus a header of meta data (indecies and sizes) 
				//To get a packet we need the INDEX and SIZE of that packet.
				int indx = packets.get_packet_offset_index(N);
				int size = packets.get_packet_size(N);

				if(size == 2){
					// packets.Buffer[indx] equals lowByte(x1) and packets.Buffer[indx+1] equals highByte(x2)
					val1 =packets.Buffer[indx];
					val2 =packets.Buffer[indx+1];
					theta = val1;
					//theta=theta+1;
					t = theta / 255.0 * 2 * Mathf.PI;
					cube.transform.position = new Vector3 (5*Mathf.Cos((float)t), 0, 5*Mathf.Sin ((float)t));
					//#########Converting val1, val2 into something similar to Input.GetAxis (Which is from -1 to 1)#########
					//since any val is from 0 to 1023

					logsOnScreen.text =  val1 + "," + val2;
					//MoveCube(Axis1,Axis2);
					/*
					 * 
					 * Now Axis1 or Axis2  value will be in the range -1...1. Similar to Input.GetAxis
					 * Check out :
					 * 
					 * https://docs.unity3d.com/ScriptReference/Input.GetAxis.html
					 * 
					 * https://unity3d.com/learn/tutorials/topics/scripting/getaxis
					 */

				}


			}

			yield return null;
		}

		statusText.text = "Status : Done Reading";

	}


}
