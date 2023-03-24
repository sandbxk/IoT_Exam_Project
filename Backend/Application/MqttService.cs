using Application.Interfaces;
using Domain;

namespace Application;

public class MqttService : IMqttService
{
    private readonly IMqttRepository _repository;

    public MqttService(IMqttRepository repository)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
    }


    public Mqtt create(Mqtt mqtt)
    {
        if (mqtt == null)
        {
            throw new NullReferenceException();
        }
        
        Mqtt returnMqtt = _repository.create(mqtt);
        
        if (returnMqtt == null)
        {
            throw new NullReferenceException();
        }
        
        return returnMqtt;
    }

    public List<Mqtt> readAll()
    {
        throw new NotImplementedException();
    }

    public Mqtt read(int id)
    {
        throw new NotImplementedException();
    }

    public Mqtt update(Mqtt mqtt)
    {
        throw new NotImplementedException();
    }

    public bool delete(int id)
    {
        throw new NotImplementedException();
    }
}