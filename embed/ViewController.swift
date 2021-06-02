//
//  ViewController.swift
//  embed
//
//  Created by Phumpakorn Saranunt on 31/5/21.
//  Copyright © 2021 Phumpakorn Saranunt. All rights reserved.
//

import UIKit
import CocoaMQTT

class ViewController: UIViewController {
    
    var mqtt: CocoaMQTT!
    var toggleState: Bool = true
    
    @IBOutlet var back: UIView!
    
    @IBOutlet weak var button: UIButton!
    
    @IBAction func cancel(_ sender: UIButton) {
        UIView.animate(withDuration: 0.3){
            self.button.transform = .identity
        }
    }
    
    @IBAction func buttonRelease(_ sender: UIButton) {
        UIView.animate(withDuration: 0.3){
            self.button.transform = .identity
            if(self.toggleState){
                self.back.backgroundColor = UIColor.black
                self.button.backgroundColor = UIColor.white
            }else{
                self.back.backgroundColor = UIColor.white
                self.button.backgroundColor = UIColor.black
            }
        }
        self.mqtt.publish("@msg/togstatus", withString: "tog")
        self.toggleState = !self.toggleState
    }
    
    @IBAction func buttonTouch(_ sender: UIButton) {
        UIView.animate(withDuration: 0.3){
        self.button.transform = CGAffineTransform(scaleX:0.95, y:0.95)
        }
        
    }
    
    func setUpMQTT() {
        let clientID = "14d81564-a5ac-41eb-9cd1-9d5df4cdfd6e"
        mqtt = CocoaMQTT(clientID: clientID, host: "broker.netpie.io", port: 1883)
        mqtt.username = "fwZKpCurBoquFXN84UJbnw8qx1mWwayB"
        mqtt.password = "womcEsEoiOwxIL)twJb!37IAI-iAD-qf"
        mqtt.willMessage = CocoaMQTTWill(topic: "/will", message: "lol")
        mqtt.keepAlive = 60
        mqtt.delegate = self
        mqtt.connect()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        setUpMQTT()
    }
}

extension ViewController: CocoaMQTTDelegate{
    /// MQTT connected with server
    // deprecated: instead of `mqtt(_ mqtt: CocoaMQTT, didConnectAck ack: CocoaMQTTConnAck)`
    func mqtt(_ mqtt: CocoaMQTT, didConnect host: String, port: Int){
        print("connected to" + host + "at port" + String(port))
    }
    func mqtt(_ mqtt: CocoaMQTT, didConnectAck ack: CocoaMQTTConnAck){
        
    }
    func mqtt(_ mqtt: CocoaMQTT, didPublishMessage message: CocoaMQTTMessage, id: UInt16){
        print("Publish Success" + message.topic)
    }
    func mqtt(_ mqtt: CocoaMQTT, didPublishAck id: UInt16){
        
    }
    func mqtt(_ mqtt: CocoaMQTT, didReceiveMessage message: CocoaMQTTMessage, id: UInt16 ){
        
    }
    // deprecated!!! instead of `func mqtt(_ mqtt: CocoaMQTT, didSubscribeTopic topics: [String])`
    func mqtt(_ mqtt: CocoaMQTT, didSubscribeTopic topic: String){
        
    }
    func mqtt(_ mqtt: CocoaMQTT, didSubscribeTopic topics: [String]){
        
    }
    func mqtt(_ mqtt: CocoaMQTT, didUnsubscribeTopic topic: String){
        
    }
    func mqttDidPing(_ mqtt: CocoaMQTT){
        
    }
    func mqttDidReceivePong(_ mqtt: CocoaMQTT){
        
    }
    func mqttDidDisconnect(_ mqtt: CocoaMQTT, withError err: Error?){
        
    }
    
}
