using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class StateText : MonoBehaviour
{
    //��Ԗ�
    string[] state;
    //��Ԃ��擾����I�u�W�F�N�g
    private PlayerController playerController;


    // Start is called before the first frame update
    void Start()
    {
        state = new string[3];

        state[0] = new string("Normal");
        state[1] = new string("Glider");
        state[2] = new string("Ship");

        playerController = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
    }

    // Update is called once per frame
    void Update()
    {
        //��Ԃ��ƂɃe�L�X�g�̕ω�
        if(playerController.glider && !playerController.ship)
        {
            this.gameObject.GetComponent<Text>().text = state[1];
        }
        else if(!playerController.glider && playerController.ship)
        {
            this.gameObject.GetComponent<Text>().text = state[2];
        }
        else
        {
            this.gameObject.GetComponent<Text>().text = state[0];
        }
    }
}
