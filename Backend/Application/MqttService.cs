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
        List<Mqtt> mqttList = _repository.readAll();
        
        if (mqttList == null)
        {
            throw new NullReferenceException();
        }

        return mqttList;
    }

    public Mqtt read(int id)
    {
        if (id <= 0)
        {
            throw new ArgumentOutOfRangeException();
        }
        
        Mqtt returnMqtt = _repository.read(id);
        
        if (returnMqtt == null)
        {
            throw new NullReferenceException();
        }

        return returnMqtt;
    }

    public Mqtt update(Mqtt mqtt)
    {
        throw new NotImplementedException();
    }

    public bool delete(Mqtt mqtt)
    {
        if (mqtt == null)
        {
            throw new NullReferenceException();
        }
        
        if (mqtt.Id <= 0)
        {
            throw new ArgumentOutOfRangeException();
        }
        
        int returnInt = _repository.delete(mqtt.Id);
        
        if (returnInt == 0)
        {
            return false;
        }
        return true;
    }
}