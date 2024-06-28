using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;

public class ScoreCountUp : MonoBehaviour
{
    //�X�R�A
    int score = 0;
    //�e�L�X�g���b�V��
    TextMeshProUGUI textMeshProUGUI;
    //�ړ�
    float time = 0.0f;

    //����l�Ɖ����l�̐ݒ�
    Vector2 min = new Vector2(0, 0);
    Vector2 max = new Vector2(100, 0);

    // Start is called before the first frame update
    void Start()
    {
       

        textMeshProUGUI = GetComponent<TextMeshProUGUI>();
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;

        score = (int) Vector2.Lerp(min, max, time).x;

        textMeshProUGUI.text = score.ToString();
    }
}
