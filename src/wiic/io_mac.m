/*
 *	io_mac.m
 *
 *	Written By:
 *		Gabriele Randelli	
 *		Email: < randelli (--AT--) dis [--DOT--] uniroma1 [--DOT--] it >
 *
 *	Copyright 2010
 *
 *	This file is part of wiiC.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief Handles device I/O for Mac.
 */
#ifdef MACOSX

#import "io_mac.h"

// Used just to retrieve max num of wiimotes in handler functions
static int max_num_wiimotes = 0;

@implementation WiiSearch

#pragma mark -
#pragma mark WiiSearch
- (id) init
{
	self = [super init];
	foundWiimotes = 0;
	if (self != nil) { 
		/* 
		 * Calling IOBluetoothLocalDeviceAvailable has two advantages:
		 * 1. it sets up a event source in the run loop (bug for C version of the bluetooth api)
		 * 2. it checks for the availability of the BT hardware
		 */
		if (!IOBluetoothLocalDeviceAvailable ())
		{
			[self release];
			self = nil;
		}		
	}

	return self;
}

- (void) dealloc
{	
	WIIUSE_DEBUG("Wiimote Discovery released");
	[super dealloc];
}

- (BOOL) isDiscovering
{
	return isDiscovering;
}

- (void) setIsDiscovering:(BOOL) flag
{
	isDiscovering = flag;
}

- (void) setWiimoteStruct:(wiimote**) wiimote_struct
{
	wiimotes = wiimote_struct;
}

- (int) getFoundWiimotes
{
	return foundWiimotes;
}

- (IOReturn) start:(unsigned int) timeout maxWiimotes:(unsigned int) wiimotes
{
	if (!IOBluetoothLocalDeviceAvailable()) {
		WIIUSE_ERROR("Unable to find any bluetooth receiver on your host.");
		return kIOReturnNotAttached;
	}
	
	// If we are currently discovering, we can't start a new discovery right now.
	if ([self isDiscovering]) {
		WIIUSE_INFO("Wiimote search is already in progress...");
		return kIOReturnSuccess;
	}
	
	[self close];
	maxWiimotes = wiimotes;
	foundWiimotes = 0;

	inquiry = [IOBluetoothDeviceInquiry inquiryWithDelegate:self];
	// We set the search timeout
	if(timeout && timeout < 20)
		[inquiry setInquiryLength:timeout];
	else
		[inquiry setInquiryLength:20];
	[inquiry setSearchCriteria:kBluetoothServiceClassMajorAny majorDeviceClass:WM_DEV_MAJOR_CLASS minorDeviceClass:WM_DEV_MINOR_CLASS];
	[inquiry setUpdateNewDeviceNames:NO];

	IOReturn status = [inquiry start];
	if (status == kIOReturnSuccess) {
		[inquiry retain];
	} else {
		[inquiry close];
		WIIUSE_ERROR("Unable to search for bluetooth devices.");
	}
	
	return status;
}

- (IOReturn) stop
{
	return [inquiry stop];
}

- (IOReturn) close
{
	IOReturn ret = kIOReturnSuccess;
	
	ret = [inquiry stop];
	[inquiry release];
	inquiry = nil;
	
	WIIUSE_DEBUG(@"Discovery closed");
	return ret;
}

#pragma mark -
#pragma mark IOBluetoothDeviceInquiry delegates
//*************** HANDLERS FOR WIIUSE_FIND FOR MACOSX *******************/
- (void) retrieveWiimoteInfo:(IOBluetoothDevice*) device
{	
	// We set the device reference (we must retain it to use it after the search)
	wiimotes[foundWiimotes]->device = [[device retain] getDeviceRef];
	wiimotes[foundWiimotes]->address = (CFStringRef)[[device getAddressString] retain];
	WIIMOTE_ENABLE_STATE(wiimotes[foundWiimotes], WIIMOTE_STATE_DEV_FOUND);
	WIIUSE_INFO("Found Wiimote (%s) [id %i].",CFStringGetCStringPtr(wiimotes[foundWiimotes]->address, kCFStringEncodingMacRoman),wiimotes[foundWiimotes]->unid);
	++foundWiimotes;
}

- (void) deviceInquiryStarted:(IOBluetoothDeviceInquiry*) sender
{
	[self setIsDiscovering:YES];
}

- (void) deviceInquiryDeviceFound:(IOBluetoothDeviceInquiry *) sender device:(IOBluetoothDevice *) device
{
	if(foundWiimotes < maxWiimotes)
		[self retrieveWiimoteInfo:device];
	else
		[inquiry stop];
}

- (void) deviceInquiryComplete:(IOBluetoothDeviceInquiry*) sender error:(IOReturn) error aborted:(BOOL) aborted
{	
	// The inquiry has completed, we can now process what we have found
	[self setIsDiscovering:NO];

	// We stop the search because of errors
	if ((error != kIOReturnSuccess) && !aborted) {
		foundWiimotes = 0;
		[inquiry close];
		CFRunLoopStop(CFRunLoopGetCurrent());
		WIIUSE_ERROR("Search not completed, because of unexpected errors. This error can be due to a short search timeout.");
		return;
	}
	
	foundWiimotes = [[inquiry foundDevices] count];
	WIIUSE_INFO("Found %i Wiimote device(s).", foundWiimotes);
	CFRunLoopStop(CFRunLoopGetCurrent());
}

@end

@implementation WiiConnect
#pragma mark -
#pragma mark WiiConnect
- (id) init
{
	self = [super init];
	receivedMsg = [[NSData alloc] init];
	_wm = 0;
	return self;
}

- (void) dealloc
{	
	WIIUSE_DEBUG("Wiimote released");
	if(receivedMsg)
		[receivedMsg release];
	receivedMsg = 0;
	_wm = 0;
	[super dealloc];
}

- (byte*) getNextMsg
{
	if(!receivedMsg)
		return 0;

	return [receivedMsg bytes];
}

- (IOBluetoothL2CAPChannel*) openL2CAPChannelWithPSM:(BluetoothL2CAPPSM) psm device:(IOBluetoothDevice*) device delegate:(id) delegate
{
	IOBluetoothL2CAPChannel* channel = nil;
	
	if ([device openL2CAPChannelSync:&channel withPSM:psm delegate:delegate] != kIOReturnSuccess) 
		channel = nil;
	
	return channel;
}

- (IOReturn) connectToWiimote:(wiimote*) wm
{
	IOBluetoothDevice* device = [IOBluetoothDevice withDeviceRef:wm->device];
	IOBluetoothL2CAPChannel* outCh = nil;
	IOBluetoothL2CAPChannel* inCh = nil;
	
	if(!device) {
		WIIUSE_ERROR("Non existent device or already connected.");
		return kIOReturnBadArgument;
	}
	
	outCh = [self openL2CAPChannelWithPSM:WM_OUTPUT_CHANNEL device:device delegate:self];
	if (!outCh) {
		WIIUSE_ERROR("Unable to open L2CAP output channel (id %i).", wm->unid);
		[self closeConnection];
		return kIOReturnNotOpen;
	}
	wm->outputCh = [[outCh retain] getL2CAPChannelRef];
	usleep(20000);
	
	inCh = [self openL2CAPChannelWithPSM:WM_INPUT_CHANNEL device:device delegate:self];
	if (!inCh) {
		WIIUSE_ERROR("Unable to open L2CAP input channel (id %i).", wm->unid);
		[self closeConnection];
		return kIOReturnNotOpen;
	}
	wm->inputCh = [[inCh retain] getL2CAPChannelRef];
	usleep(20000);
	
	IOBluetoothUserNotification* disconnectNotification = [device registerForDisconnectNotification:self selector:@selector(disconnected:fromDevice:)];
	if(!disconnectNotification) {
		WIIUSE_ERROR("Unable to register disconnection handler (id %i).", wm->unid);
		[self closeConnection];
		return kIOReturnNotOpen;
	}
	
	// We store the reference to its relative structure (Used for completing the handshake step)
	_wm = wm;
	
	return kIOReturnSuccess;
}

#pragma mark -
#pragma mark IOBluetoothL2CAPChannel delegates
- (void) disconnected:(IOBluetoothUserNotification*) notification fromDevice:(IOBluetoothDevice*) device
{
	// If the device disconnected, we also close this part of the connection
	//wiiuse_disconnect(_wm) ;

	// The wiimote_t struct must be re-initialized due to the disconnection
	wiiuse_disconnected(_wm) ;
}

//*************** HANDLERS FOR WIIUSE_IO_READ FOR MACOSX *******************/
- (void) l2capChannelData:(IOBluetoothL2CAPChannel*) channel data:(byte *) data length:(NSUInteger) length 
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	int i;

	// This is done in case the output channel woke up this handler
	if(!data) {
	    CFRunLoopStop(CFRunLoopGetCurrent());
		[pool release];
		return;
	}

	/* 
	 * This is called if we are receiving data before completing
	 * the handshaking, hence before calling wiiuse_poll
	 */
	if(WIIMOTE_IS_SET(_wm, WIIMOTE_STATE_HANDSHAKE))
		propagate_event(_wm, data[1], data+2);

	receivedMsg = [[NSData dataWithBytes:data length:sizeof(data)] retain];
	
    // Stop the main loop after reading
    CFRunLoopStop(CFRunLoopGetCurrent());
	[pool release];
}

- (void) l2capChannelReconfigured:(IOBluetoothL2CAPChannel*) l2capChannel
{
      NSLog(@"l2capChannelReconfigured");
}

- (void) l2capChannelWriteComplete:(IOBluetoothL2CAPChannel*) l2capChannel refcon:(void*) refcon status:(IOReturn) error
{
      NSLog(@"l2capChannelWriteComplete");
}

- (void) l2capChannelQueueSpaceAvailable:(IOBluetoothL2CAPChannel*) l2capChannel
{
      NSLog(@"l2capChannelQueueSpaceAvailable");
}

- (void) l2capChannelOpenComplete:(IOBluetoothL2CAPChannel*) l2capChannel status:(IOReturn) error
{
	NSLog(@"l2capChannelOpenComplete (PSM:0x%x)", [l2capChannel getPSM]);
}


@end

#pragma mark -
#pragma mark Wiiuse
/**
 *	@brief Find a wiimote or wiimotes.
 *
 *	@param wm			An array of wiimote_t structures.
 *	@param max_wiimotes	The number of wiimote structures in \a wm.
 *	@param timeout		The number of seconds before the search times out.
 *
 *	@return The number of wiimotes found.
 *
 *	@see wiimote_connect()
 *
 *	This function will only look for wiimote devices.						\n
 *	When a device is found the address in the structures will be set.		\n
 *	You can then call wiimote_connect() to connect to the found				\n
 *	devices.
 */
int wiiuse_find(struct wiimote_t** wm, int max_wiimotes, int timeout) 
{
	int found_wiimotes = 0;

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	WiiSearch* search = [[WiiSearch alloc] init];
	[search setWiimoteStruct:wm];
	[search start:timeout maxWiimotes:max_wiimotes];
	
	CFRunLoopRun();
	found_wiimotes = [search getFoundWiimotes];

	[search release];
	[pool release];
	
	return found_wiimotes;
}


//*************** HANDLERS FOR WIIUSE_DISCONNECT FOR MACOSX *******************/
/**
 *	@brief Disconnect a wiimote.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	@see wiiuse_connect()
 *
 *	Note that this will not free the wiimote structure.
 */
void wiiuse_disconnect(struct wiimote_t* wm) 
{
	IOReturn error;
	
    if (!wm || !WIIMOTE_IS_CONNECTED(wm))
            return;

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	// Input Channel
	if(wm->inputCh) {
		IOBluetoothL2CAPChannel* inCh = [IOBluetoothL2CAPChannel withL2CAPChannelRef:wm->inputCh];
		error = [inCh closeChannel];
		[inCh setDelegate:nil];
		if(error != kIOReturnSuccess) 
			WIIUSE_ERROR("Unable to close input channel (id %i).", wm->unid);			
		usleep(10000);
		[inCh release];
		inCh = nil;
		wm->inputCh = 0;
	}
	
	// Output Channel
	if(wm->outputCh) {
		IOBluetoothL2CAPChannel* outCh = [IOBluetoothL2CAPChannel withL2CAPChannelRef:wm->outputCh];
		error = [outCh closeChannel];
		[outCh setDelegate:nil];
		if(error != kIOReturnSuccess) 
			WIIUSE_ERROR("Unable to close output channel (id %i).", wm->unid);			
		usleep(10000);
		[outCh release];
		outCh = nil;
		wm->outputCh = 0;
	}
	
	// Device
	if(wm->device) {
		IOBluetoothDevice* device = [IOBluetoothDevice withDeviceRef:wm->device];
		error = [device closeConnection];
		[device setDelegate:nil];
		if(error != kIOReturnSuccess) 
			WIIUSE_ERROR("Unable to close the device connection (id %i).", wm->unid);			
		usleep(10000);
		[device release];
		device = nil;
		wm->device = 0;
	}

	// FIXME va tolto anche il disconnetion notifier
	// FIXME va tolto anche il connect
	
	[pool release];
	
	wm->event = WIIUSE_NONE;

    WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_CONNECTED);
    WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_HANDSHAKE);
}

/**
 *	@brief Connect to a wiimote with a known address.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param address	The address of the device to connect to.
 *					If NULL, use the address in the struct set by wiiuse_find().
 *
 *	@return 1 on success, 0 on failure
 */
static int wiiuse_connect_single(struct wiimote_t* wm, char* address) 
{
	// Skip if already connected or device not found
	if(!wm || WIIMOTE_IS_CONNECTED(wm) || wm->device == 0) {
		WIIUSE_ERROR("Non existent device or already connected.");
		return 0;	
	}

	// Convert the IP address
	// FIXME - see if it is possible

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	WiiConnect* connect = [[WiiConnect alloc] init];
	if([connect connectToWiimote:wm] == kIOReturnSuccess) {
		WIIUSE_INFO("Connected to wiimote [id %i].", wm->unid);
		// This is stored to retrieve incoming data
		wm->connectionHandler = (void*)([connect retain]);

		// Do the handshake 
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_CONNECTED);
		wiiuse_handshake(wm, NULL, 0);
		wiiuse_set_report_type(wm);
		
		[pool release];
	}
	else {
		[pool release];
		return 0;
	}
	
	return 1;
}

/**
 *	@brief Connect to a wiimote or wiimotes once an address is known.
 *
 *	@param wm			An array of wiimote_t structures.
 *	@param wiimotes		The number of wiimote structures in \a wm.
 *
 *	@return The number of wiimotes that successfully connected.
 *
 *	@see wiiuse_find()
 *	@see wiiuse_connect_single()
 *	@see wiiuse_disconnect()
 *
 *	Connect to a number of wiimotes when the address is already set
 *	in the wiimote_t structures.  These addresses are normally set
 *	by the wiiuse_find() function, but can also be set manually.
 */
int wiiuse_connect(struct wiimote_t** wm, int wiimotes) 
{
	int connected = 0;
	int i = 0;
	
	for (; i < wiimotes; ++i) {
		if(!(wm[i])) {
			WIIUSE_ERROR("Trying to connect more Wiimotes than initialized");
			return;
		}
		
		if (!WIIMOTE_IS_SET(wm[i], WIIMOTE_STATE_DEV_FOUND))
			// If the device address is not set, skip it 
			continue;

		if (wiiuse_connect_single(wm[i], NULL))
			++connected;
	}

	return connected;
}


int wiiuse_io_read(struct wiimote_t* wm) 
{
    if (!WIIMOTE_IS_SET(wm, WIIMOTE_STATE_CONNECTED))
            return 0;

    /* If this wiimote is not connected, skip it */
    if (!WIIMOTE_IS_CONNECTED(wm))
            return 0;

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
    // Run the main loop to get bt data
    CFRunLoopRun();

	if(!(wm->connectionHandler)) {
		WIIUSE_ERROR("Unable to find the connection handler (id %i).", wm->unid);
		return 0;
	}
	WiiConnect* deviceHandler = (WiiConnect*)(wm->connectionHandler);
	byte* buffer = [deviceHandler getNextMsg];

	if(!buffer)
		return 0;

	if(sizeof(buffer) < sizeof(wm->event_buf)) 
		memcpy(wm->event_buf,buffer,sizeof(buffer));
	else {
		WIIUSE_DEBUG("Received data are more than the buffer.... strange! (id %i)", wm->unid);
		memcpy(wm->event_buf,buffer,sizeof(wm->event_buf));
	}

	[pool release];
		
    return 1;
}


int wiiuse_io_write(struct wiimote_t* wm, byte* buf, int len) 
{
	unsigned int length = (unsigned int)len;
	
	// Small check before writing
	if(!wm || !(wm->outputCh)) {
		WIIUSE_ERROR("Attempt to write over non-existent channel (id %i).", wm->unid);
		perror("Error Details");
		return 0;
	}

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	IOBluetoothL2CAPChannel* channel = [IOBluetoothL2CAPChannel withL2CAPChannelRef:wm->outputCh];
    IOReturn error = [channel writeSync:buf length:length];
	if (error != kIOReturnSuccess) 
		WIIUSE_ERROR("Unable to write over the output channel (id %i).", wm->unid);		
    usleep(10000);
	
    [pool release];

	return (error == kIOReturnSuccess ? len : 0);
}

#endif 
