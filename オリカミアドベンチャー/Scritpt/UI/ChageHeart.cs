using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChageHeart : MonoBehaviour
{
    //��̃n�[�g�̉摜
    public Sprite emptyHeartImage;
    //�v���C���[�̏��
    private PlayerController playerState;
    public List<GameObject> images = new List<GameObject>(3);
    //�Q�[���I�[�o�[UI
    public GameObject gameOverUI;

    private void Start()
    {
        playerState = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
    }

    // Update is called once per frame
    void Update()
    {
        //�v���C���[�̗̑͂��O���傫���ꍇ
        if(playerState.HP > 0 && playerState.HP < 3)
        { 
            //�摜�����ւ���
            images[playerState.HP].GetComponent<Image>().sprite = emptyHeartImage;
        }
        if(playerState.HP <= 0)
        {
            gameOverUI.SetActive(true);
        }
    }
}
