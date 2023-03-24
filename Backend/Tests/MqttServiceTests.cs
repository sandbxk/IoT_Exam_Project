using Application;
using Application.Interfaces;
using Domain;
using FluentAssertions;
using Moq;
using MQTTnet.Server;

namespace Tests;

public class MqttServiceTests
{
    [Fact]
    public void MQTTService_WithNullAsRepository_ShouldThrowNullException()
    {
        // Arrange
        IMqttRepository repository = null;

        // Act
        Action action = () => new MqttService(repository);

        // Assert
        action.Should().Throw<ArgumentNullException>();
    }
    
    [Fact]
    public void MQTTService_WithRepository_ShouldNotThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();

        // Act
        Action action = () => new MqttService(repository.Object);

        // Assert
        action.Should().NotThrow<ArgumentNullException>();
    }
    
    [Fact]
    public void Create_WithNullAsMqtt_ShouldThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = null;

        // Act
        Action action = () => service.create(mqtt);

        // Assert
        action.Should().Throw<NullReferenceException>();
    }
    
    [Fact]
    public void Create_WithMqtt_ShouldNotThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.create(It.IsAny<Mqtt>())).Returns(new Mqtt());
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();

        // Act
        Action action = () => service.create(mqtt);

        // Assert
        action.Should().NotThrow<NullReferenceException>();
    }
    
    [Fact]
    public void Create_WithMqtt_ShouldReturnMqtt()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.create(It.IsAny<Mqtt>())).Returns(new Mqtt());
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();

        // Act
        Mqtt returnMqtt = service.create(mqtt);

        // Assert
        returnMqtt.Should().BeOfType<Mqtt>();
    }
    
    [Fact]
    public void ReadAll_WithNullAsMqttList_ShouldThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.readAll()).Returns((List<Mqtt>) null);
        MqttService service = new MqttService(repository.Object);

        // Act
        Action action = () => service.readAll();

        // Assert
        action.Should().Throw<NullReferenceException>();
    }
    
    [Fact]
    public void Read_WithIdLessThanOne_ShouldThrowOutOfRangeException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        MqttService service = new MqttService(repository.Object);
        int id = 0;

        // Act
        Action action = () => service.read(id);

        // Assert
        action.Should().Throw<ArgumentOutOfRangeException>();
    }
    
    [Fact]
    public void Read_WithIdGreaterThanZero_ShouldNotThrowOutOfRangeException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.read(It.IsAny<int>())).Returns(new Mqtt());
        MqttService service = new MqttService(repository.Object);
        int id = 1;

        // Act
        Action action = () => service.read(id);

        // Assert
        action.Should().NotThrow<ArgumentOutOfRangeException>();
    }
    
    [Fact]
    public void Delete_WithNullAsMqtt_ShouldThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = null;

        // Act
        Action action = () => service.delete(mqtt);

        // Assert
        action.Should().Throw<NullReferenceException>();
    }
    
    [Fact]
    public void Delete_WithMqtt_ShouldNotThrowNullException()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.delete(It.IsAny<int>())).Returns(1);
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();

        // Act
        Action action = () => service.delete(mqtt);

        // Assert
        action.Should().NotThrow<NullReferenceException>();
    }

    [Fact]
    public void Delete_WithReturnOf0_ShouldReturnFalse()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.delete(It.IsAny<int>())).Returns(0);
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();
        mqtt.Id = 1;

        // Act
        bool result = service.delete(mqtt);

        // Assert
        result.Should().BeFalse();
    }
    
    [Fact]
    public void Delete_WithReturnOf1_ShouldReturnTrue()
    {
        // Arrange
        Mock<IMqttRepository> repository = new Mock<IMqttRepository>();
        repository.Setup(x => x.delete(It.IsAny<int>())).Returns(1);
        MqttService service = new MqttService(repository.Object);
        Mqtt mqtt = new Mqtt();
        mqtt.Id = 1;

        // Act
        bool result = service.delete(mqtt);

        // Assert
        result.Should().BeTrue();
    }
}