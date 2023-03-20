using System.Text;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Extensions.ManagedClient;
using MQTTnet.Packets;

namespace Application
{
    public interface IMqttClient
    {
        void Connect();
        void Disconnect();
        void Subscribe(string topic);
        void Unsubscribe(string topic);        
        void Publish(string topic, string payload);
    }

    public abstract class AMqttClient : IMqttClient
    {    
        private readonly ManagedMqttClient _client;

        protected CancellationTokenSource AsyncTokenSource { get; } = new CancellationTokenSource();
        
        public AMqttClient()
        {
            var factory = new MqttFactory();
            _client = new ManagedMqttClient(factory.CreateMqttClient(), factory.DefaultLogger);
            
            _client.ConnectedAsync += ConnectedAsync;
            _client.DisconnectedAsync += DisconnectedAsync;
            _client.ConnectingFailedAsync += ConnectingFailedAsync;
            _client.ConnectionStateChangedAsync += ConnectionStateChangedAsync;
            _client.ApplicationMessageSkippedAsync += MessageSkippedAsync;
            _client.ApplicationMessageReceivedAsync += MessageReceivedAsync;
            _client.ApplicationMessageProcessedAsync += MessageProcessedAsync;

            Connect();
        }

        protected abstract Task ConnectionStateChangedAsync(EventArgs arg);
        protected abstract Task ConnectedAsync(MqttClientConnectedEventArgs arg);
        protected abstract Task ConnectingFailedAsync(ConnectingFailedEventArgs arg);
        protected abstract Task DisconnectedAsync(MqttClientDisconnectedEventArgs arg);
        protected abstract Task MessageSkippedAsync(ApplicationMessageSkippedEventArgs arg);
        protected abstract Task MessageProcessedAsync(ApplicationMessageProcessedEventArgs arg);
        protected abstract Task MessageReceivedAsync(MqttApplicationMessageReceivedEventArgs arg);


        public void Connect()
        {
            Task.Run(async () => {

                if (_client.IsConnected && _client.IsStarted)
                    return;

                await _client.StartAsync(new ManagedMqttClientOptionsBuilder()
                .WithAutoReconnectDelay(TimeSpan.FromSeconds(5))
                .WithClientOptions(new MqttClientOptionsBuilder()
                    .WithProtocolVersion(MQTTnet.Formatter.MqttProtocolVersion.V500)
                    .WithClientId("API")
                    .WithConnectionUri("mqtts://mqtt.flespi.io")
                    .WithCredentials("OemzID5B5VZ9bGr5hZ328BPwIpkbjgDVkNkTgei9Z690CoTHeMAaddwJqEKzLbjT")
                    .WithCleanSession()
                    .Build())
                .Build());
            }, AsyncTokenSource.Token);
        }

        public void Disconnect()
        {
            _client.StopAsync().RunSynchronously();

            if (!this.AsyncTokenSource.IsCancellationRequested)
            {
                this.AsyncTokenSource.Cancel();
            }
        }

        public void Publish(string topic, string payload)
        {
            Task.Run(async () => {
                await _client.EnqueueAsync(new MqttApplicationMessageBuilder()
                .WithTopic(topic)
                .WithPayload(payload)
                .WithUserProperty("source", "backend")
                .Build());
            }, AsyncTokenSource.Token);
        }

        public void Subscribe(string topic)
        {
            Task.Run(async () => await _client.SubscribeAsync(topic), AsyncTokenSource.Token);
        }

        public void Unsubscribe(string topic)
        {
            Task.Run(async () => await _client.UnsubscribeAsync(topic), AsyncTokenSource.Token);
        }
    }

    public sealed class MqttClientImpl : AMqttClient 
    {
        // singleton
        private static readonly Lazy<MqttClientImpl> _instance = new Lazy<MqttClientImpl>(() => new MqttClientImpl());
        public static MqttClientImpl Instance => _instance.Value;

        private MqttClientImpl() { }

        protected override Task ConnectingFailedAsync(ConnectingFailedEventArgs arg)
        {
            Console.WriteLine($"MQTT client has been disconnected due to: {arg.Exception.Message}");
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected override Task ConnectionStateChangedAsync(EventArgs arg)
        {
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected override Task DisconnectedAsync(MqttClientDisconnectedEventArgs arg)
        {
            Console.WriteLine($"MQTT client has been disconnected due to: {arg.Reason.ToString()}"); 
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected override Task ConnectedAsync(MqttClientConnectedEventArgs arg)
        {
            // GET server settings.
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected override Task MessageSkippedAsync(ApplicationMessageSkippedEventArgs arg)
        {
            Console.WriteLine($"Skipped message: {arg.ApplicationMessage.Id} - from {arg.ApplicationMessage.ApplicationMessage.Topic}");
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected override Task MessageProcessedAsync(ApplicationMessageProcessedEventArgs arg)
        {
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected override Task MessageReceivedAsync(MqttApplicationMessageReceivedEventArgs arg)
        {       
            arg.AutoAcknowledge = false;
            arg.ReasonCode = MqttApplicationMessageReceivedReasonCode.Success;
            arg.ResponseReasonString = "ack";
            arg.ResponseUserProperties.Clear();
            arg.ResponseUserProperties.Add(new MqttUserProperty("source", "backend"));
            Console.WriteLine($"Received message: {arg.ApplicationMessage.Topic} - {Encoding.UTF8.GetString(arg.ApplicationMessage.Payload)}");
            return arg.AcknowledgeAsync(this.AsyncTokenSource.Token);
        }
    }
}