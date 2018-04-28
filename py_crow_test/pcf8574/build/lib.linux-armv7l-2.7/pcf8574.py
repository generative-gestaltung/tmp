from smbus import SMBus


class control:
    def echo(self,content):
        if(self.debug==True):
            print str(content)
    def __init__(self,bus=1,addr=0x3f,clearbyte=True,debug=False):
        self.debug=debug
        self.echo("Debug:"+str(debug))

        self.bus=bus
        self.addr=addr
        self.bus=SMBus(self.bus)

        if clearbyte:
            self.bus.write_byte(self.addr,0xff)
            self.echo("Sent clear byte")
        else:
            self.echo("Did not sent clear byte")

    def change_state(self,pin):
        act_byte=self.bus.read_byte(self.addr)
        self.echo("Actual dec:"+str(act_byte))
        self.echo("Actual bin:"+str(bin(int(act_byte))))
        self.echo("Changing state of "+str(pin))
        abin=int(str(bin(int(act_byte)))[2:])
        pins = [1,10,100,1000,10000,100000,1000000,10000000]
        a=8-pin
        b=a-1
        if(str(abin).zfill(8)[b:a]=="0"):
            bbin=int(str(abin).zfill(8))+pins[pin]
        else:
            bbin=int(str(abin).zfill(8))-pins[pin]

        self.echo("New bin:"+str(bbin))
        self.echo("New dec:"+str(int(str(bbin),2)))
        self.bus.write_byte(self.addr,int(int(str(bbin),2)))
